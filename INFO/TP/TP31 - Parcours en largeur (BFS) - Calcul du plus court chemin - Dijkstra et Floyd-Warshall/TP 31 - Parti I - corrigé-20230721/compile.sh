DEBUG="-g"
rm -f ./*.o ./*~
gcc -c ./mylist.c -o ./mylist.o -Wall $DEBUG
gcc -c ./mystack.c -o ./mystack.o -Wall $DEBUG
gcc -c ./mygraph-lst.c -o ./mygraph.o -Wall $DEBUG # pour Dijkstra, on utilise l'implémentation par listes d'adjacence
gcc -c ./myqueue.c -o ./myqueue.o -Wall $DEBUG
gcc -c ./bfs.c -o ./bfs.o -Wall $DEBUG
gcc -c ./mypqueue.c -o ./mypqueue.o -Wall $DEBUG
gcc -c ./dijkstra.c -o ./dijkstra.o -Wall $DEBUG
gcc -c ./test.c -o ./test.o -Wall $DEBUG
gcc ./mylist.o ./mystack.o ./myqueue.o ./bfs.o ./mypqueue.o ./mygraph.o ./dijkstra.o ./test.o -o test -Wall $DEBUG
rm -f ./*.o ./*~
