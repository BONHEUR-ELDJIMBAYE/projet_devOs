projet_devOps
Un projet d'implémentation d'un mini OS.
Dans mon projet, j'ai pu mettre en place les processus, leur ordonnancement, ainsi que des appels système liés aux fonctions suivantes : wait, sleep, kill, fork, terminate, block, unblock, getpid et getppid.
J'ai implémenté un mini shell (processus à part entière) à partir duquel on peut exécuter diverses commandes.

Vous remarquerez des qu'on lance le systeme, on voit les processus 1,2 et 3 s'executer (l'ordonnancement commence directement), on peut interagir avec le shell même si les processus tournent, on les voit s'executer en même temps.
Commandes disponibles :

ps : afficher la liste des processus
kill <pid> : tuer un processus
block <pid> : bloquer un processus
unblock <pid> : débloquer un processus
interrupt <pid> : interrompre un processus
start <pid> : démarrer un processus
clear : effacer l'écran
help : afficher l'aide
play : jouer au jeu du Plus ou Moins
testproc : tester l'ordonnancement des processus
shutdown | off | exit : éteindre le système
history : afficher l'historique des commandes
history -c : effacer l'historique des commandes

J'ai aussi créé un jeu de devinette (un processus à part entière, que l'on peut lancer depuis le shell en entrant la commande play).
