#!/bin/bash


# Utilisation de find pour localiser et supprimer
# -type f : cherche uniquement des fichiers
# -name "*.o" : cherche les extensions .o
# -delete : supprime les fichiers trouvés
find . -type f -name "*.o" -delete


