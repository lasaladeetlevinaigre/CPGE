DEBUG="-g"
gcc -c ./myhashtbl-lst.c -o myhashtbl-lst.o -Wall $DEBUG
gcc -c ./myhashtbl.c -o myhashtbl.o -Wall  $DEBUG
gcc -c ./dijkstra.c -o dijkstra.o -Wall  $DEBUG
gcc -c ./mylist.c -o mylist.o -Wall  $DEBUG
gcc -c ./mystack.c -o mystack.o -Wall  $DEBUG
gcc -c ./mypqueue.c -o mypqueue.o -Wall  $DEBUG
gcc -c ./mygraph-lst.c -o mygraph.o -Wall  $DEBUG
gcc -c ./mini-mappy.c -o ./mini-mappy.o -Wall  $DEBUG
gcc ./myhashtbl-lst.o ./myhashtbl.o ./mystack.o ./mylist.o ./mypqueue.o ./mygraph.o ./dijkstra.o ./mini-mappy.o -o ./mini-mappy
rm -f ./*.o ./*~
