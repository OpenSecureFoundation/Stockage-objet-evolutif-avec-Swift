# Stockage objet évolutif avec Swift
Projet de stockage objet évolutif avec OpenStack Swift

# Objectifs:

• Développer une application de sauvegarde automatique des données vers Swift avec versionning

• Mettre en place un système de réplication inter-sites pour la haute disponibilité des objets

# PROCEDURE D'INSTALLATION : 
Prerequis : Installation des dependances pour communiquer avec openstack et opensack-swift 
    . sudo apt update
    . sudo apt install python3-swiftclient python3-openstackclient

1. Preparation de l'environnement de travail
    . Creation du fichier de configuration qui contiendra les variables d'environnement indispensables a l'authentification aupres de keystone : creer un fichier user.openrc
    . Injecter grace a la commande <<source user.openrc>> ces variables dans la session actuelle de votre terminal

2. Lancer le programme


NB : un exemple du fichier de configuration est present dans le dossier config a la racine du projet 