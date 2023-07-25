import numpy as np
import matplotlib.pyplot as plt

n = np.arange(1, 101, 1)

u = np.zeros(100)

for k in n:
    if (k%2 == 0):
        u[k-1] = 1.0/k
    else:
        u[k-1] = (-1.0)/k

#un = [ if (k%2 == 0): 1/k else: (-1)/k for k in n] #np.cos(n)

plt.plot(n, u, "oc")
plt.xlabel("Indice n")
plt.ylabel("u(n)")
plt.title("Graphe de la suite u(n)= (-1)^n / n")
plt.show()


n = np.arange(0, 21, 1)
v = np.zeros(21)

v[0] = 1.4
for k in range(0, 20):
    v[k+1] = np.cos(v[k])

#print(n)

#print(un)

plt.plot(n, v, "oc")
plt.xlabel("Indice n")
plt.ylabel("v(n)")
plt.title("Graphe de la suite v(n+1)=cos(v(n))")
plt.show()    

