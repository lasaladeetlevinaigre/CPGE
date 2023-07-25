DEBUG="-g"
gcc -c ./mylist.c -o ./mylist.o -Wall $DEBUG
gcc -c ./mystack.c -o ./mystack.o -Wall $DEBUG
gcc -c ./mygraph-mat.c -o ./mygraph.o -Wall $DEBUG # pour Floyd-Warshall, on utilise l'implémentation par matrice d'adjacence
gcc -c ./floyd-warshall.c -o ./floyd-warshall.o -Wall $DEBUG
gcc ./mylist.o ./mystack.o ./mygraph.o ./floyd-warshall.o  -o fw -Wall $DEBUG
rm -f ./*.o ./*~
