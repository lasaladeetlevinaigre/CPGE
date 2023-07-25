rm -f ./*.o ./test ./*~ ./*#
#IMPLEM="./mygraph-mat.c" #attention, pas d'espaces autour du =...
IMPLEM="./mygraph-lst.c"

gcc -c ./mylist.c -o ./mylist.o -Wall 
gcc -c $IMPLEM -o ./mygraph.o -Wall 
gcc -c ./test.c -o test.o -Wall 

gcc  ./mylist.o ./mygraph.o ./test.o -o test -Wall
rm -f ./*.o
