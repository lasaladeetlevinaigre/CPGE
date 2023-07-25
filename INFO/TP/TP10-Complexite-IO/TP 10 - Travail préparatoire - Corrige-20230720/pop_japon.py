import matplotlib.pyplot as plt

annees = range(0, 60, 5) #[0, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55]
population = [44.8, 47, 50.7, 53.4, 55.9, 59.7, 64.5, 69.3, 73.1, 72.4, 83.6, 87.9]

annees = [i + 1900 for i in annees]
plt.plot(annees, population, "+r")
plt.xlabel("Année");
plt.ylabel("Population en millions")
plt.title("Evolution de la population japonaise dans la 1ere moitié du XXeme siècle")
plt.show()

