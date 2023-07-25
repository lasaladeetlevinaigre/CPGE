# compilation de l'interface abstraite/signature du module
ocamlc -c ./Wgraph.mli

#comilation de l'implémentation concrète, qui génère un fichier Wgraph.cmo qui
# contient toutes les fonctions concrètes du module
# et vérifie l'adéquation avec l'interface Wgraph.mli (les fichiers doivent avoir exactement le même nom de base
ocamlc -c ./Wgraph.ml


# Pour utiliser le module Wgraph en interactif, il faut simplement écrire
#            #use "./Wgraph.ml"
#en tête du ficher de test test.ml
