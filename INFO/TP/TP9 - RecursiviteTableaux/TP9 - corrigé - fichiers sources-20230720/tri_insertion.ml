(* On utilise des listes, plus adaptées pour la programmation récursive en OCaml *)

Random.self_init() (* initialisation automatique du germe (seed) de l'algorithme de génération de nombres pseudo-aléatoires*)

(* La fonction s'arrête si n est strictement négatif. Par contre, elle rattrape l'erreur si a > b en inversant a et b au prochain appel récursif *)
let rec genere_liste_aleatoire n a b =
  match (n, a, b) with
  | (p, _, _) when p < 0     -> failwith "le nombre d'éléments doit être positif ou nul"
  | (0, _, _)                -> [] (*cas terminal*)
  | (_, a, b) when (a <= b)  -> (a + Random.int (b-a+1))::(genere_liste_aleatoire (n-1) a b) (* cas si a <= b*)
  | _                        -> (b + Random.int (a-b+1))::(genere_liste_aleatoire (n-1) b a) (* cas si a > b*)
;;

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


tri_insertion [ 6; -5; 7; 1];;
tri_insertion [ 1; 7; 3; 2; -5; 7; 1];;
tri_insertion [ 1; ];;
tri_insertion [ 'f'; 'z'; 'a'; 'c'; 'w'];;
tri_insertion [ 1.3; -5.1; 8.1; -3.0; -4.1];;

let t = genere_liste_aleatoire 100 0 10000;;
tri_insertion t;;
