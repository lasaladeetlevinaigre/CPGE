# compilation
gcc -c ./quicksort.c -o quicksort.o -Wall
gcc -c ./time-analysis-ex4.c -o time-analysis.o -Wall
gcc quicksort.o time-analysis.o -o time-analysis -Wall

# execution du code et création du fichier avec les resultats exprimentaux
./time-analysis

# lancement du script python d'analyse
python3 ./plot_complexity.csv
