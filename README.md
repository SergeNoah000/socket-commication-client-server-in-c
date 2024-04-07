
# Projet de Communication Socket en Paires en C

Ce projet consiste en la création d'une communication entre un client et un serveur en utilisant des sockets en C. Le client peut envoyer un message au serveur, qui l'affiche en retour. Le serveur a été mis à jour pour accepter plusieurs connexions simultanément, chaque thread gérant une connexion acceptée. Le thread fonction reçoit le message envoyé par le client, récupère l'adresse IP du client, ouvre un fichier et écrit les informations (adresse IP + message) à l'intérieur. Le serveur est capable d'écouter les connexions pendant que les threads traitent les demandes des clients.

Auteur : [Noah Mvondo Serge Gaetan](https://github.com/SergeNoah000) 

## Fichiers

* client.c : fichier contenant le code du client.
* client_infinite.c : fichier contenant le code du client pour lancer des requete non stop afin de verifier le multitfhreading du serveur.
* server.c : fichier contenant le code du serveur.
* server_updated.c : fichier contenant le code du serveur pour la reception de plusieurs connexions et l'ecrire dans un fichier de log. Il contient aussi la fonctionnalité de liste du nombre de connexions actives
* Makefile : fichier permettant de compiler les deux programmes.

## Prérequis

* Un système d'exploitation prenant en charge le langage C.
* Un compilateur C.
* Les bibliothèques nécessaires pour les sockets (déjà incluses dans les fichiers source).

## Utilisation


### Cloner le projet
Cloner le dépôt GitHub :

```bash
git clone https://github.com/SergeNoah000/socket-commication-client-server-in-c.git
```

Acceder au dossier en utilisant la commande

```bash
cd socket-commication-client-server-in-c
```
### Compilation

Pour compiler les deux fichiers source, exécutez la commande :

```bash
make
```
Cela créera deux exécutables : client et server.

### Lancement du serveur

Pour lancer le serveur, exécutez la commande :

```bash
./server
```
Le serveur sera à l'écoute des connexions sur le port .

### Lancement du client

Pour lancer le client et envoyer un message au serveur, exécutez la commande :

```bash
./client
```

### Lancement du serveur multithreads
Le serveur multithreads permet de lancer plusieurs threads pour traiter chacun une connexion s'il y en a plusieurs entrantes. Il est lance en utilisant la commande:

```bash
./server_full PORT
 ```
#### Exemple
Voici un exemple d'utilisation du serveur multithreads:

```bash
./server_full  6060 
```
Arreter le avec un `CTRL + C`

### Lancement du client de requetes infinies
Commande: 

```bash
./infinite_client ADRESSE_IP_SERVER PORT 
```

#### Exemple
Voici un exemple d'utilisation du client au requetes infinies:

```bash
./infinite_client 127.0.0.0 6060 
```
Arreter le avec un `CTRL + C`


### Nettoyer le ficher de logs
Pour nettoyer les fichiers `.txt` du répertoire courant, utilisez la commande suivante :

```bash
make clean
```

Vous pouvez ouvrir plusieurs clients sur des machines différentes pour simuler plusieurs connexions. Assurez-vous que le serveur est accessible depuis les machines clientes.

### Fonctionnement

Le client envoie un message au serveur, qui l'affiche en retour. Le serveur enregistre également l'adresse IP du client et le message dans un fichier nommé client_messages.log. Chaque client peut envoyer un message au serveur, qui sera traité par un thread dédié.

## Remarques

* Pour tester plusieurs connexions, il est recommandé d'exécuter les clients sur des machines virtuelles différentes, en veillant à ce que le serveur soit accessible depuis ces machines.
* Les fichiers source sont fournis en langage C, ils peuvent donc être compilés et exécutés sur différents systèmes d'exploitation prenant en charge le langage C.
* Les fichiers source sont commentés pour faciliter la compréhension du code.



## Note
Ce projet s'inscrit dans le cadre d'apprentissage de l'implementation des systèmes d'exploitation master I 
à l'Universite de Yaoundé I Filière informatique de la faculté des Sciences | [uy1.cm](https://uy1.cm)