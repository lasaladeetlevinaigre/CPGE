import numpy as np
import matplotlib.pyplot as plt

x = np.linspace(0.1, 100, 10000, True)
f = x * np.log2(x)

#print(x)
#print(f)

plt.plot(x, f, "r-")
plt.xlabel("x")
plt.ylabel("xlog(x)")
plt.title("Graphe de la fonction f(x) = xlog2(x). Comportement asymptotique quasi-linéaire.")
plt.show()
