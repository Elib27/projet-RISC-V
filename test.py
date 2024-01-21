#! /usr/bin/env python3
import subprocess
import pytest
import shutil
import struct
import glob
import sys
import os
import re

BASE_DIR = os.environ.get("BASE_DIR", ".")
RISCV_ASSEMBLER = os.path.join(BASE_DIR, "riscv-assembler")
RISCV_EMULATOR = os.path.join(BASE_DIR, "riscv-emulator")
TEST_DIR = os.environ.get("TEST_FILES", os.path.join(BASE_DIR, "tests/*.s"))

if "TEST_FILES" in os.environ:
    ALL_FILES = glob.glob(os.environ["TEST_FILES"], recursive=True)
else:
    ALL_FILES = glob.glob(
        os.path.join(BASE_DIR, "tests/**/*.s"), recursive=True)

# Find the first program in PATH from the provided list
def find_program(options):
    return next((o for o in options if shutil.which(o) is not None), None)

# Some people have no global name for LLVM toolchains. Feel free to add to the
# end of the lists to match your setup.
CLANG = find_program(["clang", "clang-14"])
OBJCOPY = find_program(["llvm-objcopy", "llvm-objcopy-14", "riscv64-elf-objcopy"])
RV64_GCC = find_program(["riscv64-elf-gcc"])

if (CLANG is None and RV64_GCC is None) or OBJCOPY is None:
    print("error: missing tools!")
    print("- clang:", CLANG)
    print("- objcopy:", OBJCOPY)
    print("- gcc:", RV64_GCC)
    sys.exit(1)

# Split into non-empty lines, remove spaces, translate to lower case
def normalize_and_split_lines(s):
    s = [re.sub(r"[ \t\n]+", "", line.lower()) for line in s.split("\n")]
    return [line for line in s if line]

riscv_regs = [
    "zero", "ra", "sp",  "gp",  "tp", "t0", "t1", "t2",
    "s0",   "s1", "a0",  "a1",  "a2", "a3", "a4", "a5",
    "a6",   "a7", "s2",  "s3",  "s4", "s5", "s6", "s7",
    "s8",   "s9", "s10", "s11", "t3", "t4", "t5", "t6",
]

# Parse string into a final state dictionary
def parse_final_state(s):
    s = normalize_and_split_lines(s)
    state = dict()
    for line in s:
        reg, value = line.split(":", 1)
        reg = reg.strip()

        m = re.fullmatch(r"x(\d)+", reg)
        if m is not None:
            regnum = int(reg[1:])
            if regnum > 31:
                pytest.fail("Unknown register '{}' in output".format(reg))
                return
        elif reg in riscv_regs:
            regnum = riscv_regs.index(reg)
        else:
            pytest.fail("Unknown register '{}' in output".format(reg))
        state[regnum] = int(value.strip(), 0)
    return state

def assert_equal_hex(ref, out):
    if len(out) != len(ref):
        pytest.fail("Output should contain {} instructions, but has {}"
                    .format(len(ref), len(out)))
    for i in range(len(ref)):
        if out[i] != ref[i]:
            pytest.fail("Instruction #{} should be {:08x}, but is {:08x}"
                        .format(i+1, ref[i], out[i]))

def assert_equal_regs(ref, out):
    ref = parse_final_state(ref)
    out = parse_final_state(out)

    msg = ""
    failed = False

    for r in range(32):
        if ref.get(r, 0) != out.get(r, 0):
            msg += "\n- x{} ({}) should be {}, but it is {}".format(
                r, riscv_regs[r], ref.get(r, 0), out.get(r, 0))
            failed = True

    if failed:
        pytest.fail(msg)

def convert_gcc_syntax(input, output):
    with open(input, "r") as fp:
        asm = fp.read()

    RE_JUMP = re.compile(r'\b(j|jal|beq|bne|blt|bge)\b\s*([^\n]+)', re.I)
    def rep(m):
        args = m[2].split(",")
        args = args[:-1] + ["(.+ " + args[-1] + ")"]
        return m[1] + " " + ", ".join(args)

    asm = RE_JUMP.sub(rep, asm)
    with open(output, "w") as fp:
        fp.write(asm)

def assembler_reference(file):
    prog = os.path.splitext(file)[0]
    prog_obj = prog + ".o"
    prog_bin = prog + ".bin"

    if RV64_GCC is not None:
        file_gcc = file[:-2] + ".s.gnu"
        convert_gcc_syntax(file, file_gcc)
        subprocess.run(
            [RV64_GCC, "-march=rv64i", "-mabi=lp64", "-x", "assembler",
             "-c", file_gcc, "-o", prog_obj],
            check=True)
    else:
        subprocess.run(
            [CLANG, "--target=riscv64", "-march=rv64g", "-c", file, "-o",
            prog_obj],
            check=True)
    subprocess.run(
        [OBJCOPY, "-O", "binary", "-j", ".text", prog_obj,
         prog_bin],
        check=True)

    with open(prog_bin, "rb") as fp:
        data = fp.read()
    assert len(data) % 4 == 0
    return struct.unpack("<{}I".format(len(data) // 4), data)

class TestRISCVAssembler:
    @pytest.mark.parametrize("filename", ALL_FILES)
    def test_assembler(self, filename):
        prog = os.path.splitext(filename)[0]
        prog_hex = prog + ".hex"
        reference = assembler_reference(filename)

        if os.path.exists(prog_hex):
            os.remove(prog_hex)

        # Run the command, ignoring I/Os (we only use output files)
        subprocess.run(
            [RISCV_ASSEMBLER, filename, prog_hex],
            timeout=10, stderr=subprocess.STDOUT, check=True)

        produced = []
        with open(prog_hex, "r") as fp:
            for line in fp.read().splitlines():
                try:
                    produced.append(int(line, 16))
                except ValueError:
                    pass

        assert_equal_hex(reference, produced)

class TestRISCVEmulation:
    def get_expected(self, file):
        expected = ""
        inside = False

        with open(file, encoding="utf-8") as fp:
            for line in fp.readlines():
                # Ignore non-comments
                if not re.match(r"\s*#", line):
                    continue
                # Cleanup comment start and whitespaces
                line = re.sub(r"\s*#\s*", "", line)
                line = re.sub(r"\s*$", "", line)

                if line == "END":
                    inside = False
                elif line == "EXPECTED":
                    inside = True
                elif inside:
                    expected += "\n" + line

        # Make it None if it's an empty string
        return expected or None

    @pytest.mark.parametrize("filename", ALL_FILES)
    def test_emulator(self, filename):
        prog, _ = os.path.splitext(filename)
        prog_s = prog + ".s"
        prog_hex = prog + ".ref.hex"
        prog_state = prog + ".state"

        expected = self.get_expected(filename)

        ref_hex = assembler_reference(prog_s)
        with open(prog_hex, "w") as fp:
            for h in ref_hex:
                fp.write("{:08x}\n".format(h))

        if os.path.exists(prog_state):
            os.remove(prog_state)

        # Run the command, ignoring I/Os (we only use output files)
        subprocess.run(
            [RISCV_EMULATOR, prog_hex, prog_state],
            timeout=10, stderr=subprocess.STDOUT, check=True)

        if expected is None:
            pytest.skip("no EXPECTED block in test file")
        if not os.path.exists(prog_state):
            pytest.skip("emulation not implemented yet")
        with open(prog_state, "r") as fp:
            state = fp.read()
        if state == "":
            pytest.skip(".state file empty")
        assert_equal_regs(expected, state)

pytest.main(sys.argv)
