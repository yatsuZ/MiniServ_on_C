Sujet
Nom de la tâche : mini_serv
Fichiers attendus : mini_serv.c
Fonctions autorisées : write, close, select, socket, accept, listen, send, recv, bind, strstr, malloc, realloc, free, calloc, bzero, atoi, sprintf, strlen, exit, strcpy, strcat, memset

Écrire un programme qui attendra la connexion d'un client sur un port spécifique du réseau 127.0.0.1 et permettra aux clients de communiquer entre eux.

Ce programme prendra comme premier argument le port auquel se connecter.
Si aucun argument n'est fourni, il doit écrire dans la sortie standard « Nombre d'arguments incorrect » suivi d'un \n et quitter avec le statut 1.
Si un appel système renvoie une erreur avant que le programme n'accepte la connexion, il doit écrire dans la sortie standard « Erreur fatale » suivi d'un \n et quitter avec le statut 1.
Si vous ne pouvez pas allouer de mémoire, il doit écrire dans la sortie standard « Erreur fatale » suivi d'un \n et quitter avec le statut 1. 1

Votre programme doit être non bloquant, mais le client peut être paresseux. S'il ne lit pas votre message, vous ne devez PAS le déconnecter.

Votre programme ne doit pas contenir de #define preproc.
Votre programme doit uniquement écouter 127.0.0.1.
Le fd que vous recevrez sera déjà configuré pour bloquer « recv » ou « send » si select n'a pas été appelé avant, mais ne bloquera pas autrement.

Lorsqu'un client se connecte au serveur :

Un identifiant lui sera attribué. Le premier client recevra l'identifiant 0, puis chaque nouveau client recevra l'identifiant du dernier client + 1.
%d sera remplacé par ce numéro.
Un message est envoyé à tous les clients connectés au serveur : « serveur : le client %d vient d'arriver\n ».

Les clients doivent pouvoir envoyer des messages à votre programme. - Le message ne contient que des caractères imprimables, aucune vérification nécessaire.
- Un même message peut contenir plusieurs \n.
- Lorsque le serveur reçoit un message, il doit le renvoyer à tous les autres clients en ajoutant « client %d : » avant chaque ligne !

Lorsqu'un client se déconnecte du serveur :
- Un message est envoyé à tous les clients connectés : « serveur : client %d vient de quitter\n ».

Les fuites de mémoire ou de fichiers sont interdites.

Pour vous aider, vous trouverez le fichier main.c contenant le début d'un serveur et peut-être quelques fonctions utiles. (Attention : ce fichier utilise des fonctions interdites ou contient des éléments qui ne doivent pas figurer dans votre programme final.)

Attention : notre testeur s'attend à ce que vous envoyiez les messages le plus rapidement possible. N'utilisez pas de tampon inutile.

L'évaluation peut être un peu plus longue que d'habitude...

Astuce : vous pouvez utiliser nc pour tester votre programme
Astuce : vous devriez utiliser nc pour tester votre programme
Astuce : Pour tester, vous pouvez utiliser fcntl(fd, F_SETFL, O_NONBLOCK) mais utilisez select et ne cochez JAMAIS EAGAIN (man 2 send).