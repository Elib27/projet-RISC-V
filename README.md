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

Pour l'analyse de texte, nous avons décidé d'utiliser la commande getline() qui nous retourne la figne entière. Nous avons ensuite remplacé les virgules et parenthèses par des espaces pour nettoyer l'instruction. Nous avons ensuite extrait l'instruction en la sauvgardant dans une variable. Ceci rendra plus simple la recherche de l'instruction parmis toutes celles implémentées. Pour identifier les noms de registres, des noms "jolis", des entiers, on teste si l'argument est un nom de registre, puis un entier, sinon c'est un nom "joli".


* Avez-vous vu des motifs récurrents émerger ? Avez-vous "factorisé" ces motifs
pour éviter de les répéter ? Si non, serait-ce difficile ?

A la lecture du sujet, nous avons remarqué que beaucoup d'instructions se ressemblent. En effet, les instructions de même type ont sensiblement la même structure de données. Ceci nous a permis de les regrouper ensembles. Nous avons donc 5 facons d'encoder nous instructions au lieu d'une facon par instruction. Cela permet de simplifier grandement le code et notament la répétitivité du code.

* Comment avez-vous procédé pour écrire les tests ? Étes-vous confiant·e·s que
toutes les instructions gérées et tous les types d'arguments sont couverts ?

[COMPLÉTER ICI]

* Quelle a été votre expérience avec l'utilisation et la compréhension de la
documentation fournie sur l'encodage des instructions RISC-V ?

Nous avons bien compris la documentation RISC-V fournie. L'encodage des instructions est claire et permet tout de suite de bien comprendre le fonctionnement de l'encodage RISC-V.

* Cochez (en remplaçant `[ ]` par `[x]`) si vous avez :
  - [x] Implémenté la traduction pour toutes les instructions de la documentation
  - [ ] Pris en compte les cas particuliers comme les valeurs immédiates négatives et le bit manquant dans l'encodage de `jal`
  - [ ] Écrit du code de gestion d'erreur pour que le programme ait une réaction propre si le code assembleur est invalide _(pas demandé par le sujet)_


## Rendu 3

Questions à remplir _avant_ de programmer l'émulateur (10 lignes sont conseillées à chaque fois pour bien y réfléchir) :

* Listez tous les éléments matériels auxquels vous pouvez penser dont l'émulateur doit reproduire le comportement, et déduisez-en une liste de toutes les tâches individuelles de l'émulateur.


  L'émulateur doit gérer :

  - Une pile contenant les instructions de tout le programme ainsi que les valeurs stockées par le programme
  - Une banque de 32 registres
  - Un CPU, qui a plusieurs fonctions :
    - Récuperer l'instruction présente dans la mémoire à l'offset du pointeur d'instruction
    - La décoder
-
* Quelle fonction de la bibliothèque standard pouvez-vous utiliser pour lire les valeurs listées dans le fichier `.hex` sans vous casser la tête ? (Indice : ces valeurs ont été écrites avec `fprintf()`.)

[COMPLÉTER ICI]

* Décrivez comment vous allez répartir les tâches de l'émulateur en différents fichiers, ou ne pas les répartir et tout faire dans le même fichier. Expliquez les avantages de votre choix.

[COMPLÉTER ICI]

Questions à remplir _après_ avoir programmé l'émulateur :

* Aviez-vous réussi à listé toutes les tâches dans la première question ? Rétrospectivement, y a-t-il des tâches dont vous aviez sous-estimé ou sur-estimé la complexité ?

[COMPLÉTER ICI]

* Avez-vous compris le fonctionnement de chaque instruction à partir de la
documentation fournie ? Si non, quels sont les points obscurs ?

[COMPLÉTER ICI]

* Quels exemples de programmes avez-vous choisi pour tester le calcul ? Les
comparaisons et sauts ? La mémoire ?

[COMPLÉTER ICI]

* Reste-t-il des bugs que vous avez découverts et pas corrigés ?

[COMPLÉTER ICI]

* D'autres remarques sur votre programme ?

[COMPLÉTER ICI]

* Cochez (en remplaçant `[ ]` par `[x]`) si vous avez :**
  - [ ] Implémenté l'émulation de toutes les instructions gérées par le rendu 2.
  - [ ] Implémenté l'émulation de toutes les instructions.
  - [ ] Tous vos tests qui passent.
  - [ ] Vérifié que vous tests couvrent toutes les instructions émulées.
  - [ ] Testé les cas particuliers : valeurs négatives, overflows...
  - [ ] Testé les cas d'erreur : division par zéro, sauts invalides... _(pas demandé par le sujet)_
  - [ ] Un port fonctionnel de DOOM pour votre émulateur.

* Des retours sur le projet en général ?

[COMPLÉTER ICI]
