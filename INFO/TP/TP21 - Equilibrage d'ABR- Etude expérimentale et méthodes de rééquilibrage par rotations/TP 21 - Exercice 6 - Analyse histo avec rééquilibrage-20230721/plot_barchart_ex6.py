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
path_unbalanced = "./random_bst_unbalanced.csv"

h,n,n_nodes = read_csv(path)
print(n_nodes)
plt.bar(h,n, color='b')

h,n,n_nodes = read_csv(path_unbalanced)
plt.bar(h,n, color='r')

b = np.ceil((np.log(n_nodes+1))/(np.log( (1+np.sqrt(5))/2 ) ))-1
print("Max hauteur possible theorique pour "+str(n_nodes)+" noeuds, arbre AVL: "+str(b))


plt.plot([b, b], [0, max(n)], 'g--')

b = np.ceil(np.log2(n_nodes+1))-1
print("Meilleure hauteur possible pour "+str(n_nodes)+" noeuds, arbre equilibré: "+str(b))

plt.plot([b, b], [0, max(n)], 'g--')

plt.xlabel("Hauteur")
plt.ylabel("Nombre d'arbres à "+str(n_nodes)+" noeuds")
plt.title("Diagramme en bâtons: nombre d'arbres pour une hauteur donnee")
plt.show()


    
    
