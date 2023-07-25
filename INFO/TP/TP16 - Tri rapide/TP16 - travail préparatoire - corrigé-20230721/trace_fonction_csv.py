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
    return x, fx

path = "./valeurs_fonction.csv"

x,fx = read_csv(path)

plt.plot(x, fx, "r+-", label="f")

plt.xlabel("x")
plt.ylabel("f(x)")
plt.title("Tracé de la fonction issue du fichier " + path)
plt.show()


    
    
