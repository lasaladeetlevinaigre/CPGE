gcc -c ./mylist.c -o mylist.o -Wall
gcc -c ./bintree-ex3.c -o bintree.o -Wall
gcc mylist.o bintree.o -o exe_bintree
