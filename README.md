# Projet_laby
Mes fichiers du projet labyrinthe de Ravensburger:
un makefile, clientAPI.c, labyrinthAPI.c, main.c (contenant l'ensemble des programmes) et labyrinthe.c (contenant le tp5 labyrinthe)

#Ligne de commande pour la compilation sans makefile
gcc -Wall -o main main.c clientAPI.c labyrinthAPI.c

#programme
le programme commence par définir toutes les variables donc il a besoin notamment les allocations dynamiques, il récupère les données depuis le serveur puis les réarrange pour pouvoir les utiliser plus simplement (un tableau de tuile- 'tab-tuile'), puis un deuxième réarrangement qui fini par donnée un un tableau statique 'tab_affiche' qui tranforme les données recues du serveur en tableau équivalent au tp labyrinthe.
Il y également une 'fonction' qui permet de mettre un jour le labyrinthe (tab_tuile - un tableau constitué de tuile)
en continuant mon programme j'ai rencontré un problème (erreur de segmentation) qui est soit liée au réarrangement des tableaux soit au fonction de mise à jour (il y a moins de chance pour cette deuxieme hypothese.)

N'ayant pas pu trouvé cette erreur, je n'ai malheuresement pas pu me consacré à l'élaboration du bot.
