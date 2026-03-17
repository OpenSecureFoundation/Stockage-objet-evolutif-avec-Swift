# Stockage objet évolutif avec Swift
Projet de stockage objet évolutif avec OpenStack Swift

# Objectifs:

• Développer une application de sauvegarde automatique des données vers Swift avec versionning

• Mettre en place un système de réplication inter-sites pour la haute disponibilité des objets

# PREREQUIS
Avant de commencer, assurez-vous d'avoir les dépendances nécessaires pour communiquer avec le Cloud OpenStack 
    . sudo apt update
    . sudo apt install python3-swiftclient python3-openstackclient
Configurer le fichier /etc/hosts en ajoutant le ligne :  
    . IP_ADDRESS  controller (adresse ip du controller openstack)


# PROCEDURE D'INSTALLATION ET DE CONFIGURATION 

    - Étape 1 : Compilation du Programme
        . Placez-vous à la racine du projet et compilez le binaire : make clean et make
        . Une fois compilé, déplacez le binaire vers un répertoire système pour qu'il soit accessible par le service avec la commande : sudo cp syncora /usr/local/bin/syncora

    - Étape 2 : Mise en place du Service
        . Recuperez le fichier de service dans le dossier config du repository et placé le dans le dossier : /etc/systemd/system/ avec la commande : sudo nano /etc/systemd/system/syncora.service.
        . Changer le nom du fichier : sudo cp syncora.service.sample  syncora.service ( retirer le .sample du nom) 

    - Etape 3 : Configuration des accès OpenStack
        . Modifiez la section [service] du fichier : modifier les variables User et Group.
        . Modifiez la section "VARIABLES OPENSTACK" avec vos informations d'authentification a Keystone.
        . Modifiez la variable " WorkingDirectory = " en mettant le chemin d'acces au projet C_syncora_v2 cloné sur votre machine locale 
    
    - Etape 4 : Activation du service
        . Saisissez les commandes suivantes : sudo systemctl daemon-reload, sudo systemctl enable syncora.service, sudo systemctl start syncora.service
    
# UTILISATION 
    L'application va automatiquement créer un dossier dans ~/Documents/Syncora/ et creer autant de dossier que de conteneurs associé au compte swift mentionné dans les configurations OpenStack.Tout fichier ajouté, modifié ou supprimé dans ces dossiers sera synchronisé en temps réel.

# LOGS 
    - Pour vérifier si la synchronisation se passe bien ou diagnostiquer une erreur il est possible de voir les logs en direct en ouvrant le terminal et saisissant la commande : journalctl -u syncora.service -f
    - Il est aussi possible de verifier le statut du service pour savoir s'il a bien demarré avec la commande : systemctl status syncora.service

# NB : 
    1. Un exemple du fichier de configuration des variables d'environnement est present dans le dossier config a la racine du projet.
    2. Le fichier de service : syncora.service est present dans le dossier config du repository.