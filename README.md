# Projet RISC-V CS351

Auteurs : BAS Eliot et ARNAUD-GODDET Etienne

## Rendu 1

* Cochez (en remplaçant `[ ]` par `[x]`) si vous avez :
  - [x] Vérifié que `make test` se plaint sur le nombre d'instructions et pas
      sur l'existence des fichiers de sortie.
  - [x] Vu sur Chamilo que les soumissions se font avec `make tar`.


## Rendu 2

(Une dizaine de lignes devrait suffire à chaque fois)

* Comment avez-vous choisi de programmer l'analyse de texte (dont la lecture
des opérandes entières) ? Comment identifiez-vous les noms de registres, des noms "jolis", des entiers ?

Pour l'analyse de texte, nous avons décidé d'utiliser la commande `getline()` qui nous retourne la ligne entière. On remplace les virgules et parenthèses par des espaces pour nettoyer l'instruction. On extrait ensuite l'instruction et les arguments dans des variables avec la fonction sscanf. Ceci rendra plus simple la recherche de l'instruction parmis toutes celles implémentées. Pour identifier les noms de registres, des noms "jolis", des entiers, on teste si l'argument est un nom de registre, puis un entier, sinon c'est un nom "joli".


* Avez-vous vu des motifs récurrents émerger ? Avez-vous "factorisé" ces motifs
pour éviter de les répéter ? Si non, serait-ce difficile ?

A la lecture du sujet, nous avons remarqué que beaucoup d'instructions se ressemblaient. En effet, les instructions de même type ont sensiblement la même structure. Ceci nous a permis de les regrouper les instructions d'un même type dans une même fonction. Nous avons donc 5 facons d'encoder nos instructions au lieu d'une facon différente par instruction. Il suffit simplement de tester quelle est l'instruction dans la fonction pour appliquer les changements spécifiques à cette instruction si nécessaire. Cela permet de simplifier grandement le code et notament la répétition du code.

* Comment avez-vous procédé pour écrire les tests ? Étes-vous confiant·e·s que
toutes les instructions gérées et tous les types d'arguments sont couverts ?

Concernant les tests, nous avons testé chaque instruction avec des arguments différents. Tous les types d'arguments: registres, les valeurs immédiates positives et négatives (valeurs tstées aux bornes). Tous les registres ont été testés avec leur alias et leurs numéro pour bien assurer leur reconnaissance par le programme.

* Quelle a été votre expérience avec l'utilisation et la compréhension de la
documentation fournie sur l'encodage des instructions RISC-V ?

Nous avons bien compris la documentation RISC-V fournie. L'encodage des instructions est claire et permet tout de suite de bien comprendre le fonctionnement de l'encodage RISC-V.

* Cochez (en remplaçant `[ ]` par `[x]`) si vous avez :
  - [x] Implémenté la traduction pour toutes les instructions de la documentation
  - [x] Pris en compte les cas particuliers comme les valeurs immédiates négatives et le bit manquant dans l'encodage de `jal`
  - [ ] Écrit du code de gestion d'erreur pour que le programme ait une réaction propre si le code assembleur est invalide _(pas demandé par le sujet)_


## Rendu 3

Questions à remplir _avant_ de programmer l'émulateur (10 lignes sont conseillées à chaque fois pour bien y réfléchir) :

* Listez tous les éléments matériels auxquels vous pouvez penser dont l'émulateur doit reproduire le comportement, et déduisez-en une liste de toutes les tâches individuelles de l'émulateur.


  L'émulateur doit gérer :

  - Une mémoire contenant les instructions de tout le programme ainsi que les valeurs stockées par le programme
  - Une banque de 32 registres
  - Un CPU, qui a plusieurs fonctions
  
  Les tâches de l'émulateurs sont les suivantes :
    - Récuperer l'instruction présente dans la mémoire à l'offset du pointeur d'instruction
    - La décoder, récuperer les bons paramètres en fonctions de l'instruction
    - Lire les bonnes valeurs dans la mémoire si besoin
    - Executer les changements sur les registres
    - Appliquer les changements sur la mémoire


* Quelle fonction de la bibliothèque standard pouvez-vous utiliser pour lire les valeurs listées dans le fichier `.hex` sans vous casser la tête ? (Indice : ces valeurs ont été écrites avec `fprintf()`.)

Pour lire le fichier `.hex`, nous allons utilliser l'instruction `fscanf()` avec le format %08x. Cela va nous retourner les instrutions sous forme d'entiers que nous pourons manipuler ensuite dans le décodage de notre émulateur.

* Décrivez comment vous allez répartir les tâches de l'émulateur en différents fichiers, ou ne pas les répartir et tout faire dans le même fichier. Expliquez les avantages de votre choix.

Comme le programme est court, on choisit de laisser les différentes tâches de l'émulateur dans un seul et même fichier pour une meilleure structure et clarté du programme.

Questions à remplir _après_ avoir programmé l'émulateur :

* Aviez-vous réussi à listé toutes les tâches dans la première question ? Rétrospectivement, y a-t-il des tâches dont vous aviez sous-estimé ou sur-estimé la complexité ?

Dans la première question, nous avons listé toutes les grandes étapes de l'émulation. Nous avons sous-estimé les problèmes des négatifs dans les immédiats. En effet, il faut pouvoir détecter la présence d'un immédiat négatif pour bien pouvoir faire l'extension du signe.

* Avez-vous compris le fonctionnement de chaque instruction à partir de la
documentation fournie ? Si non, quels sont les points obscurs ?

Oui, la documentation était assez claire et la première partie sur l'assembleur nous a déjà permis de bien comprendre le fonctionnement de chaque instruction.

* Quels exemples de programmes avez-vous choisi pour tester le calcul ? Les
comparaisons et sauts ? La mémoire ?

Nous avons choisi de tester le calcul avec des additions, des soustractions, avec des valeurs immédiates et des registres. Nous avons testé les comparaisons avec des instructions de comparaisons et des sauts. Nous avons testé la mémoire avec des instructions de chargement et de stockage. On n'oublie pas de tester les pseudo-instructions.

* Reste-t-il des bugs que vous avez découverts et pas corrigés ?

Nous avons corrigés tous les bugs que nous avons découverts.

* D'autres remarques sur votre programme ?

[COMPLÉTER ICI]

* Cochez (en remplaçant `[ ]` par `[x]`) si vous avez :**
  - [x] Implémenté l'émulation de toutes les instructions gérées par le rendu 2.
  - [x] Implémenté l'émulation de toutes les instructions.
  - [x] Tous vos tests qui passent.
  - [x] Vérifié que vous tests couvrent toutes les instructions émulées.
  - [x] Testé les cas particuliers : valeurs négatives, overflows...
  - [ ] Testé les cas d'erreur : division par zéro, sauts invalides... _(pas demandé par le sujet)_
  - [ ] Un port fonctionnel de DOOM pour votre émulateur.

* Des retours sur le projet en général ?

Ce projet nous a permis de mieux comprendre le fonctionnement d'un processeur, spécifiquement l'encodage, le décodage et l'exécution de l'assembleur (ici RISC-V). Nous avons pu voir comment un processeur gère la mémoire et les registres.
Ce fut un projet très intéressant mais peut-être un peu petit pour y travailler en binôme.