(* ex1 *)

(*q1*)
type 'a bintree = E | N of ('a bintree) * 'a * ('a bintree);;

(*q2*)

let arbre_enonce = N(N(E, 1, E), 2, N(N(E, 4, E), 3, N(E, 5, E)));;

(*q3*)
let peigne_gauche = N(N(N(N(E, 3, E), 4, E), 2, E), 1, E);;
let peigne_droit = N(E, 1, N(E, 2, N(E, 3, N(E, 4, E))));;

(*q4*)

let rec bintree_number_of_nodes t =
  match t with
  |E -> 0
  |N(l, _, r) -> 1 + (bintree_number_of_nodes l) + (bintree_number_of_nodes r)
;;

bintree_number_of_nodes arbre_enonce;;
bintree_number_of_nodes peigne_gauche;;
bintree_number_of_nodes peigne_droit;;



(*q5*)
let rec bintree_height t =
  match t with
  | E -> -1
  | N(l, _, r) -> 1+ max (bintree_height l) (bintree_height r)
;;

bintree_height arbre_enonce;;
bintree_height peigne_gauche;;
bintree_height peigne_droit;;

(*q6*)
let rec bintree_leaves t =
  match t with
  | N(E, _, E) -> 1
  | N(l, _, E) -> (bintree_leaves l)
  | N(E, _, r) -> (bintree_leaves r)
  | N(l, _, r) -> (bintree_leaves l) + (bintree_leaves r)
  |_ -> failwith "On ne doit pas arriver ici..."
;;

bintree_leaves arbre_enonce;;
bintree_leaves peigne_gauche;;
bintree_leaves peigne_droit;;

(*q7*)
let rec bintree_max t =
  match t with
  |N(E, v, E) -> v
  |N(l, v, E) -> max v (bintree_max l)
  |N(E, v, r) -> max v (bintree_max r) 
  |N(l, v, r) -> max v (max (bintree_max l) (bintree_max r))
  |_ -> failwith "On ne doit pas arriver ici..."
;;

bintree_max arbre_enonce;;
bintree_max peigne_gauche;;
bintree_max peigne_droit;;


(*q8*)
let rec bintree_sum t =
  match t with
  |N(E, v, E) -> v
  |N(l, v, E) ->  v + (bintree_sum l)
  |N(E, v, r) -> v + (bintree_sum r) 
  |N(l, v, r) -> v + (bintree_sum l) + (bintree_sum r)
  |_ -> failwith "On ne doit pas arriver ici..."
;;

bintree_sum arbre_enonce;;
bintree_sum peigne_gauche;;
bintree_sum peigne_droit;;


(* ex2 *)

(*q1*)
let rec bintree_preorder t =
  match t with
  |E -> ()
  |N(l, v, r) -> print_char ' '; print_int v;  bintree_preorder l; bintree_preorder r
;;

bintree_preorder arbre_enonce;;
bintree_preorder peigne_gauche;;
bintree_preorder peigne_droit;;

(*q2*)
let rec bintree_inorder t =
  match t with
  |E -> ()
  |N(l, v, r) ->  bintree_inorder l; print_int v; print_char ' ';  bintree_inorder r
;;

bintree_inorder arbre_enonce;;
bintree_inorder peigne_gauche;;
bintree_inorder peigne_droit;;

let rec bintree_postorder t =
  match t with
  |E -> ()
  |N(l, v, r) -> bintree_postorder l; bintree_postorder r;  print_int v; print_char ' '
;;

bintree_postorder arbre_enonce;;
bintree_postorder peigne_gauche;;
bintree_postorder peigne_droit;;

(*q3 et 4*)

let rec bintree_preorder_list t=
  match t with
  |E -> [ ]
  |N(l, v, r) -> (v::bintree_preorder_list l)@ (bintree_preorder_list r)
;;

bintree_preorder_list arbre_enonce;;
bintree_preorder_list peigne_gauche;;
bintree_preorder_list peigne_droit;;

let bintree_preorder_list_version2 t =
let rec bintree_preorder_aux t acc =
  match t with
  |E -> acc
  |N(l, v, r) -> bintree_preorder_aux r (bintree_preorder_aux l (v::acc))
in
List.rev (bintree_preorder_aux t [])
;;

bintree_preorder_list arbre_enonce;;
bintree_preorder_list_version2 arbre_enonce;;
bintree_preorder_list peigne_gauche;;
bintree_preorder_list_version2 peigne_gauche;;
bintree_preorder_list peigne_droit;;
bintree_preorder_list_version2 peigne_droit;;

let rec bintree_inorder_list t=
  match t with
  |E -> [ ]
  |N(l, v, r) -> (bintree_inorder_list l)@ (v::(bintree_inorder_list r))
;;

bintree_inorder_list arbre_enonce;;
bintree_inorder_list peigne_gauche;;
bintree_inorder_list peigne_droit;;

let bintree_inorder_list_version2 t =
let rec bintree_inorder_aux t acc =
  match t with
  |E -> acc
  |N(l, v, r) -> bintree_inorder_aux r (v::(bintree_inorder_aux l acc))
in
List.rev (bintree_inorder_aux t [])
;;

bintree_inorder_list arbre_enonce;;
bintree_inorder_list_version2 arbre_enonce;;
bintree_inorder_list peigne_gauche;;
bintree_inorder_list_version2 peigne_gauche;;
bintree_inorder_list peigne_droit;;
bintree_inorder_list_version2 peigne_droit;;

let rec bintree_postorder_list t=
  match t with
  |E -> [ ]
  |N(l, v, r) -> (bintree_postorder_list l)@ (bintree_postorder_list r)@[v]
;;

bintree_postorder_list arbre_enonce;;
bintree_postorder_list peigne_gauche;;
bintree_postorder_list peigne_droit;;

let bintree_postorder_list_version2 t =
let rec bintree_postorder_aux t acc =
  match t with
  |E -> acc
  |N(l, v, r) -> v::(bintree_postorder_aux r (bintree_postorder_aux l acc))
in
List.rev (bintree_postorder_aux t [])
;;

bintree_postorder_list arbre_enonce;;
bintree_postorder_list_version2 arbre_enonce;;
bintree_postorder_list peigne_gauche;;
bintree_postorder_list_version2 peigne_gauche;;
bintree_postorder_list peigne_droit;;
bintree_postorder_list_version2 peigne_droit;;

(* q5 aucune de ces fonctions n'est terminale: on doit effectuer des opérations utilisant les retours des appels récursifs... *)

(* ex3 *)

(*q1*)
let bintree_max_sum t =
  let rec poids t acc =
    match t with
    | E -> acc
    | N(l,v,r)-> max (poids l (acc+v)) (poids r (acc+v))
  in match t with
     |E -> failwith "Arbre vide!"
     |N(l, v, r) -> max (poids l v) (poids r v)
;;


bintree_max_sum arbre_enonce;;
bintree_max_sum peigne_gauche;;

let bintree_max_sum_path t =
  let rec poids t acc acc_l =
    match t with
    | E -> (acc, acc_l)
    | N(l,v,r)-> let (pg,lg) = (poids l (acc+v) (v::acc_l)) and
                        (pd,ld) = (poids r (acc+v) (v::acc_l)) in
                    if (pg > pd) then (pg, lg) else (pd, ld)
  in match t with
     |E -> failwith "Arbre vide!"
     |N(l, v, r) -> let (pg,lg) = (poids l v [v]) and (pd, ld) = (poids r v [v]) in
                       if (pg > pd) then (List.rev lg) else (List.rev ld)
;;

bintree_max_sum_path arbre_enonce;;
bintree_max_sum_path peigne_gauche;;

(*ex4*)

let rec bintree_create_perfect h =
  if (h = -1) then
    E
  else
    N(bintree_create_perfect (h-1), h, bintree_create_perfect (h-1))
;;

let t = bintree_create_perfect 3;;
bintree_preorder t;;

let rec bintree_is_perfect t =
  let n = bintree_number_of_nodes t and
      h = bintree_height t in
  (n = int_of_float(2. ** float_of_int(h+1))-1 )
(* t est un arbre parfait ssi on a exactement n= 2^{h+1}-1. Dans le cours, on a montrer =>, il faut montrer la réciproque...
   On peut le faire en montrer la propriété P(k): Si t est un arbre binaire à 2^{k+1}-1 noeuds, alors t est un arbre binaire parfait de hauteur k
   P(-1) est triviale car l'abrre vide est parfait
   P(0) aussi car l'arbe à un noeud est parfait
   Supposons P(k) vraie
   Soit un arbre t possédant n= 2^{k+2}-1 noeuds. n = 2^{k+1}-1 + 2^{k+1} = arbre bianire à (2^{k+1}-1) noeuds, auquel on a ajouté  2^{k+1} noeuds de plus
   Il s'agit donc d'un arbre binaire parfait  de hauteur k (par hypothèse de récurrence), auquel on a rajouté 2^{k+1} noeuds de plus, c'est à dire un étage complet en plus
   Ainsi, t est un arbre binaire parfait de hauteur (k+1) CQFD
 *)
;;


let arbre_parfait = N( N(N(E, 4, E), 2, N(E, 5, E)) , 1,  N(N(E, 6, E), 3, N(E, 7, E)));;
bintree_is_perfect arbre_parfait;;

let arbre_complet = N( N(N(E, 4, E), 3, N(E, 5, E)) , 2, N(E, 1, E) );;
bintree_is_perfect arbre_complet;;

(* Une fausse bonne idée *)
let rec bintree_is_full_fonction_fausse t =
  let n = bintree_number_of_nodes t and
      h = bintree_height t in
  (n <= int_of_float(2. ** float_of_int(h+1))-1 ) && (n >= int_of_float(2. ** float_of_int(h))-1 ) (* Ceci est une condition nécessaire mais pas suffisante pour être complet, car un arbre complet ne doit pas avoir de "trou" dans son dernier étage: tout doit etre rempli en partant de la gauche *)
;;


bintree_is_full arbre_parfait;;
bintree_is_full arbre_complet;;
bintree_is_full peigne_gauche;;


let arbre_non_complet = N( N(E, 3, N(E, 5, E)) , 2, N(E, 1, E) );;
bintree_is_full_fonction_fausse arbre_non_complet;; (* ne donne pas la bonne réponse! l'arbre n'est pas complet car le dernier niveau a un trou tout à gauche! *)

(* idée: il faut utiliser un accumulateur de type string  qui stocke par exemple 0 qd on va a gauche et 1 à droite. On peut utiliser la concaténation de chaine ^ pour faire cela. Quand on arrive à une feuille, on stocke le chemin formé de 0 et de 1 dans une liste et ensuite, il faut vérifier que tous les chemins obtenus sont des entiers consécutifs à partir de 0. A vous de jouer *)
