#creation du premier fichier objet, qui est  au format binaire ELF
gcc -c ./fichier1.c -o fichier1.o
readelf -s ./fichier1.o > table-des-symboles-1.txt

gcc -c ./fichier2.c -o fichier2.o
readelf -s ./fichier2.o > table-des-symboles-2.txt

#edition de liens
gcc fichier1.o fichier2.o -o test_exe

#nettoyage
rm ./*.o ./*~

#execution du fichier binaire exécutable final, dont toutes les dépendances on été résolues
./test_exe
