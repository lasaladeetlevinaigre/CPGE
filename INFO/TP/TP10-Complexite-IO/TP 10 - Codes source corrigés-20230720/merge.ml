Random.self_init() (* initialisation automatique du germe (seed) de l'algorithme de génération de nombres pseudo-aléatoires*)


let rec genere_liste_alea a b n =
  match n with
  | n when n < 0 -> failwith "Taille negative"
  | 0            -> []
  | _ when a > b -> (b + Random.int (a - b + 1))::(genere_liste_alea b a (n-1)) (*résilience du code si l'utilisateur donne a > b*)
  | _            -> (a + Random.int (b - a + 1))::(genere_liste_alea a b (n-1))
;;

(* precondition: l1 et l2 sont triées *)
let rec merge l1 l2 =
  match (l1, l2) with
  | ([], []) -> []
  | ([], _) -> l2
  | (_, []) -> l1
  | (t1::q1, t2::q2) when (t1 < t2) -> t1::(merge q1 l2)
  | (t1::q1, t2::q2)                -> t2::(merge l1 q2)
;;

let l1 = [1;3;5];;
let l2 = [2;4;6;8;10];;
merge l1 l2;;

let l1 = ['b'; 'b'; 'e'; 'k'; 'p'];;
let l2 = ['a'; 'a'; 'g'; 'z'];;
merge l1 l2;;

let rec tri_insertion l =
  (* on définit insere comme une fonction auxiliaire 
     locale à la fonction principale, 
     mais cela n'est pas obligatoire *)
  let rec insere v l_trie =
    match l_trie with
    | [] -> [v]
    | h::t when v < h -> v::l_trie
    | h::t -> h :: (insere v t)
  in
  match l with
  |[] -> []
  |h::t -> insere h (tri_insertion t)
;;

let l1 = tri_insertion (genere_liste_alea 0 30 10);;
let l2 = tri_insertion (genere_liste_alea 0 50 15);;
merge l1 l2;;

let l1 = ['b'; 'b'; 'e'; 'k'; 'p'];;
let l2 = ['a'; 'a'; 'g'; 'z'];;
merge l1 l2;;



let  merge_terminale l1 l2 =
  let rec aux l_acc l1 l2 = 
  match (l1, l2) with
  | ([], []) -> List.rev l_acc
  | ([], _) -> (List.rev l_acc)@l2
  | (_, []) -> (List.rev l_acc)@l1
  | (t1::q1, t2::q2) when (t1 < t2) -> aux (t1::l_acc) q1 l2
  | (t1::q1, t2::q2)                -> aux (t2::l_acc) l1 q2
  in
  aux [] l1 l2
;;

let l1 = [1;3;5];;
let l2 = [2;4;6;8;10];;
merge_terminale l1 l2;;

let l1 = ['b'; 'b'; 'e'; 'k'; 'p'];;
let l2 = ['a'; 'a'; 'g'; 'z'];;
merge_terminale l1 l2;;


let l1 = tri_insertion (genere_liste_alea 0 30 10);;
let l2 = tri_insertion (genere_liste_alea 0 50 15);;
merge_terminale l1 l2;;


