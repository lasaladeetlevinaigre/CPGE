(* structure d'arbre général *)
type 'a tree = Node of 'a * 'a tree list;;

(* structure d'arbre généralCalcul du nombre de noeuds. Algorithme récursif avec récursivité croisée *)
let rec tree_number_of_nodes t =
  match t with
  |Node(_, forest) -> 1 + forest_number_of_nodes forest
and forest_number_of_nodes f =
  match f with
  |[] -> 0
  |head_tree::tail_forest -> (tree_number_of_nodes head_tree) + ( forest_number_of_nodes tail_forest)
;;

(* construction de l'arbre exemple de l'énoncé *)
let test = Node('A', [
                      Node('B',
                           [
                             Node('D', [])
                           ]
                          );
                      Node('C', [
                             Node('E', []);
                             Node('F', [
                                          Node('H', [])
                                       ]
                                 );
                             Node('G', [])
                            ]
                           )
                       ]
               );;

tree_number_of_nodes test;;
 
(* strategie avec un accumulateur partagé entre les deux fonctions mais attention, la fonction est
   toujours non récursive terminale *)
let tree_number_of_nodes t =
  let rec aux_tree_number_of_nodes t acc =
    match t with
    |Node(_, forest) -> aux_forest_number_of_nodes forest (acc+1)
  and aux_forest_number_of_nodes f acc =
    match f with
    |[] -> acc
    |head_tree::tail_forest ->  aux_forest_number_of_nodes tail_forest (aux_tree_number_of_nodes head_tree acc)
  in
  aux_tree_number_of_nodes t 0
;;


tree_number_of_nodes_term test;;


(* calcul de la hauteur d'un arbre général *)
let rec tree_height t =
  match t with
  |Node(_, f) -> 1 + (forest_height f)
and forest_height f =
  match f with
  |[] -> -1
  |head_tree::tail_forest -> max (tree_height head_tree) (forest_height tail_forest)
;;
tree_height test;;

(* version avec accumulateurs mais toujours non recursive terminale *)
let tree_height t =
  let rec aux_tree_height t acc =
    match t with
    |Node(_, []) -> acc
    |Node(_, f) -> (aux_forest_height f (acc+1))
  and aux_forest_height f acc =
    match f with
    |[] -> acc
    |head_tree::tail_forest ->  aux_forest_height tail_forest (max acc (aux_tree_height head_tree acc))
  in
  aux_tree_height t (-1)
;;
tree_height test;;


(* Parcours d'arbres généraux *)

(* Premiere version
let rec preorder t =
  preorder_forest [t] 
and preorder_forest f =
  match f with
  |[] -> []
  |Node(v, f1)::f2 -> (v::(preorder_forest f1))@(preorder_forest f2)
;;

preorder tree_test;; *)

let rec tree_preorder f (Node(v, forest)) =
  f v;
  List.iter (tree_preorder f) forest
;;

(* on passe la fonction en argument, en utilisant une définition de fonction anonyme avec le mot clé fun *)
tree_preorder (fun c -> print_char c) test;;

let rec tree_postorder f (Node(v, forest)) =
  List.iter (tree_postorder f) forest;
  f v
;;

tree_postorder (fun c -> print_char c) test;;


(* Conversion depuis et vers un arbre binaire *)
type 'a bintree = E | N of 'a bintree * 'a * 'a bintree;;
(* attention, le constructeur de noeuds s'appelle N pour les arbres binaires et Node pour les arbres généraux... *)


(* conversion tree -> bintree *)
let rec bintree_of_tree t =
  bintree_of_forest [t]
and bintree_of_forest f =
  match f with
  |[] -> E
  |Node(v, f1)::f2 -> N(bintree_of_forest f1 ,v, bintree_of_forest f2) (* les frères f2 "tombent" dans le sous arbre droit *)
;;

bintree_of_tree test;;

(* conversion réciproque bintree -> tree *)
let rec tree_of_bintree_isomorphe t =
  match t with
  |E -> failwith "Un arbre général ne peut pas être vide. Impossible de convertir"
  |N(l, v, E) -> Node(v, forest_of_bintree l)
  |_ -> failwith "Impossible de convertir cet arbre binaire en arbre avec la transformation réciproque isomorphe: le sous arbre droit doit etre vide"
and forest_of_bintree t =
  match t with
  |E -> []
  |N(l, v, r) -> Node(v, forest_of_bintree l)::(forest_of_bintree r) (* le sous arbre droit "remonte" au niveau de la fratrie *)
;;

let bintest = N(N(N(E,'D',E), 'B', N(N(E, 'E', N(N(E,'H',E),'F', N(E,'G',E))), 'C', E)), 'A', E);;
let tiso = tree_of_bintree_isomorphe bintest;;
let resiso  = bintree_of_tree tiso;;

(* conversion réciproque bintree -> tree générale, qui n'est PAS la fonction réciproque de tree_of_bintree car plus générale: elle accepte en entrée les arbre binaires dans la racine a un sous arbre droit non vide*)
let rec tree_of_bintree_non_isomorphe t =
  match t with
  |E -> failwith "Un arbre général ne peut pas être vide. Impossible de convertir"
  |_ -> match forest_of_bintree t with
        | [tt] -> tt
        | _ -> failwith "Erreur"
and forest_of_bintree t =
  match t with
  |E -> []
  |N(l, v, r) -> [Node(v, (forest_of_bintree l)@(forest_of_bintree r) )]
;;

(* On applique tree_of_bintree_non_isomorphe(b->t) rond bintree_of_tree(t->b) à l'arbre test... et on ne retombe pas sur le même arbre: la transformation n'est pas isomorphe! *)
let bintest = bintree_of_tree test;; 
tree_of_bintree_non_isomorphe bintest;; 
test;; 


(* et si on les compose dans l'autre sens, ca ne marche pas non plus *)
let b2 = N(N(N(E,'D',E), 'B', N(N(E, 'E', N(N(E,'H',E),'F', N(E,'G',E))), 'C', E)), 'A', E);;
let t2 = tree_of_bintree_non_isomorphe b2;;
let res  = bintree_of_tree t2;;
b2;;

(* que l'on mette ou non un sous arbre droit... *)
let b3 = N(N(N(E,'D',E), 'B', N(N(E, 'E', N(N(E,'H',E),'F', N(E,'G',E))), 'C', E)), 'A', N(E, 'I', E));;
let t3 = tree_of_bintree_non_isomorphe b3;;
let res  = bintree_of_tree t3;;
b3;;


(* par contre, avec la version isomorphe, on retrouve bien l'arbre de départ en composant les deux fonctions de conversion *)
let bintest = bintree_of_tree test;; 
tree_of_bintree_isomorphe bintest;; 
test;;

let b2 = N(N(N(E,'D',E), 'B', N(N(E, 'E', N(N(E,'H',E),'F', N(E,'G',E))), 'C', E)), 'A', E);;
let t2 = tree_of_bintree_isomorphe b2;;
let res  = bintree_of_tree t2;;
b2;;

(* mais il faut restreindre l'isomorphisme aux arbres binaires n'ayant pas de sous arbre droit sous la racine *)
let b3 = N(N(N(E,'D',E), 'B', N(N(E, 'E', N(N(E,'H',E),'F', N(E,'G',E))), 'C', E)), 'A', N(E, 'I', E));;
let t3 = tree_of_bintree_isomorphe b3;;
