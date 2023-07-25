gcc -g -c mylist.c -o mylist.o -g -Wall
gcc -g -c hashtbl.c -o hashtbl.o -g -Wall
gcc -g mylist.o hashtbl.o -o test -g -Wall
rm -f ./*~ ./#* ./*.o
