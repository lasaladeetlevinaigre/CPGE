import csv
import numpy as np
import matplotlib.pyplot as plt

nom_fichier = "complexite_amortie.csv"


def lire_fichier(nom_fichier):
    with open(nom_fichier, newline='') as csvfile:
        reader = csv.reader(csvfile, delimiter=',')
        k = []
        phik = []
        for ligne in reader:
            if (len(ligne)> 1):
                n = int(ligne[0]);
                k.append(n)
                t = float(ligne[1])
                phik.append(t)
        return k, phik

k, phik = lire_fichier(nom_fichier)
plt.plot(k, phik, "r+", label="tri par insertion")
plt.xlabel("Invocation")
plt.ylabel("Valeur du potentiel")
plt.title("Analyse expérimentale du potentiel")
plt.show()
