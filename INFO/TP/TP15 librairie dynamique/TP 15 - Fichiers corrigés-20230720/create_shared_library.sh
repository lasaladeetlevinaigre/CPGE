rm -f ./*.o ./*.so ./test ./*~ ./#*
gcc -c -fPIC -Wall  ./mylist.c -o ./mylist.o
gcc ./mylist.o -shared -o libmylist.so 
gcc   test.c -lmylist -o test -L. -Wall
unset LD_LIBRARY_PATH
export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH
echo $LD_LIBRARY_PATH

rm -f ./*.o  ./*~ ./#*

./test
