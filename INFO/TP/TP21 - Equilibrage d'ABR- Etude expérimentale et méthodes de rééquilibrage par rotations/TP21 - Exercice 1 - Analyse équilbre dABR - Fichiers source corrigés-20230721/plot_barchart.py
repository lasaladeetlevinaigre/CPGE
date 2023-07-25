import csv
import matplotlib.pyplot as plt
import numpy as np


def read_csv(path):
    with open(path, newline='') as csvfile:
        reader = csv.reader(csvfile, delimiter=",")
        x = []
        fx = []
        for line in reader:
            if (len(line) > 1):
                x.append(float(line[0]))
                fx.append(float(line[1]))
            else:
                n_nodes = float(line[0])
    return x, fx, n_nodes

path = "./random_bst.csv"


h,n,n_nodes = read_csv(path)
print(n_nodes)

plt.bar(h,n)
b = np.ceil(np.log2(n_nodes+1))-1
print("Meilleure hauteur possible pour "+str(n_nodes)+" noeuds, arbre equilibré: "+str(b))

plt.plot([b, b], [0, max(n)], 'r--')
plt.xlabel("Hauteur")
plt.ylabel("Nombre d'arbres à "+str(n_nodes)+" noeuds")
plt.title("Diagramme en bâtons: nombre d'arbres pour une hauteur donnee " + path)
plt.show()


    
    
