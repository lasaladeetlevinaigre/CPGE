import matplotlib.pyplot as plt
import numpy as np

# taille du tableau généré pour chaque exécution
n = [1000, 10000, 25000, 50000, 75000, 100000, 125000, 150000]
# temps de calcul mesuré expérimentalement pour chaque exécution
t_insertion = [0.043, 0.104, 0.380, 1.146, 3.112, 5.748, 8.553, 12.312]


plt.plot(n, t_insertion, "r+")
plt.xlabel("Taille du tableau n")
plt.ylabel("Temps d'exécution")
plt.title("Evolution du temps d'execution en fonction de la taille du tableau")
plt.show()

ncarre = [k*k for k in n] 
plt.plot(ncarre, t_insertion, "r+")
plt.xlabel("Taille du tableau au carré n*n")
plt.ylabel("Temps d'exécution")
plt.title("Mise en évidence du comportement asymptotique quadratique du tri insertion")
plt.show()




