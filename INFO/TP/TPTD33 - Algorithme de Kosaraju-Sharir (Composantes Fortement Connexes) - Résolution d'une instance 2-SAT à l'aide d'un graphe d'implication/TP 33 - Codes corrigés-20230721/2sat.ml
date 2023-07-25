#use "./Wgraph.ml"



let g2 = Wgraph.init_no_edge 6 true;;
Wgraph.add_edge g2 0 1 1.;
Wgraph.add_edge g2 1 4 1.;
Wgraph.add_edge g2 1 5 1.;
Wgraph.add_edge g2 4 2 1.;
Wgraph.add_edge g2 5 2 1.;
Wgraph.add_edge g2 2 1 1.;
Wgraph.add_edge g2 3 0 1.;
Wgraph.add_edge g2 3 2 1.;;


let g3 = Wgraph.init_no_edge 8 true;;
Wgraph.add_edge g3 0 2 1.;
Wgraph.add_edge g3 1 3 1.;
Wgraph.add_edge g3 3 6 1.;
Wgraph.add_edge g3 3 7 1.;
Wgraph.add_edge g3 4 0 1.;
Wgraph.add_edge g3 4 7 1.;
Wgraph.add_edge g3 5 1 1.;
Wgraph.add_edge g3 5 4 1.;
Wgraph.add_edge g3 5 7 1.;
Wgraph.add_edge g3 6 0 1.;
Wgraph.add_edge g3 6 2 1.;
Wgraph.add_edge g3 6 4 1.;
Wgraph.add_edge g3 7 2 1.                                
;;

let g4 = Wgraph.init_no_edge 8 true;;
Wgraph.add_edge g4 1 3 1.0;
Wgraph.add_edge g4 1 4 1.0;
Wgraph.add_edge g4 2 3 1.0;
Wgraph.add_edge g4 3 2 1.0;
Wgraph.add_edge g4 3 0 1.0;
Wgraph.add_edge g4 4 0 1.0;
Wgraph.add_edge g4 4 1 1.0;
Wgraph.add_edge g4 5 2 1.0;
Wgraph.add_edge g4 5 7 1.0;
Wgraph.add_edge g4 6 5 1.0;
Wgraph.add_edge g4 6 1 1.0;
Wgraph.add_edge g4 6 2 1.0;
Wgraph.add_edge g4 7 6 1.0;;

let g5 = Wgraph.init_no_edge 11 true;;
Wgraph.add_edge g5 0 1 1.0;
Wgraph.add_edge g5 1 3 1.0;
Wgraph.add_edge g5 2 0 1.0;
Wgraph.add_edge g5 3 7 1.0;
Wgraph.add_edge g5 3 5 1.0;
Wgraph.add_edge g5 3 4 1.0;
Wgraph.add_edge g5 3 2 1.0;
Wgraph.add_edge g5 4 6 1.0;
Wgraph.add_edge g5 5 4 1.0;
Wgraph.add_edge g5 6 5 1.0;
Wgraph.add_edge g5 7 8 1.0;
Wgraph.add_edge g5 8 9 1.0;
Wgraph.add_edge g5 9 10 1.0;
Wgraph.add_edge g5 9 7 1.0;;


let dfs_sort_terminated g =
    let nv = Wgraph.number_of_vertices g in
    let visited = Array.make nv false in
    let postorder = Stack.create () in
    
    let rec aux_dfs_rec g u  =
      Printf.printf "dfs %d\n" u; 
      visited.(u) <- true;
      List.iter (
        fun (v,w) -> Printf.printf "\tvoisin de %d: %d\n" u v;
        if not visited.(v) then
          aux_dfs_rec g v
        ) (Wgraph.succ g u);
      Printf.printf "\t... %d terminé\n" u; 
      Stack.push u postorder
      
    in
  
    for source = 0 to (nv-1) do
      if (visited.(source) = false) then
        (aux_dfs_rec g source)
    done;
    postorder
  
;;


let t = dfs_sort_terminated g2;;
Stack.iter (fun k -> Printf.printf "%d " k) t;; (* ce n'est pas un tri topologique car il y aun cycle *)

let tri_topologique = dfs_sort_terminated g3;;
Stack.iter (fun k -> Printf.printf "%d " k) tri_topologique;;


let arbre = Wgraph.init_no_edge 11 false;;
Wgraph.add_edge arbre 0 1 1.0;
Wgraph.add_edge arbre 1 2 1.0;
Wgraph.add_edge arbre 1 3 1.0;
Wgraph.add_edge arbre 3 4 1.0;
Wgraph.add_edge arbre 4 5 1.0;
Wgraph.add_edge arbre 5 6 1.0;
Wgraph.add_edge arbre 0 7 1.0;
Wgraph.add_edge arbre 7 8 1.0;
Wgraph.add_edge arbre 8 9 1.0;
Wgraph.add_edge arbre 8 10 1.0;;

let tri_topologique = dfs_sort_terminated arbre;;
Stack.iter (fun k -> Printf.printf "%d " k) tri_topologique;;

let tri_topo = dfs_sort_terminated g4;;
Stack.iter (fun k -> Printf.printf "%d " k) tri_topo;;



(* Calcul de composantes fortement connexes *)

(*La fonction connex_components ne marche pas sur les graphes orientés. Par exemple, elle ne marche pas du tout sur g3 ci-dessus! g3 a trois composantes connexes, mais si on prend le sommet 0 comme source, notre algo indique que le graphe a 2 composantes connexes !*)


                                                                                                                                                                                         



(* fonction qui inverse les arcs d'un graphe orienté *)
let reverse g =
  if (Wgraph.is_directed g = false) then
    g
  else
    (
      let nv = Wgraph.number_of_vertices g in
      let rev_g = Wgraph.init_no_edge nv true in
      for u = 0 to (nv-1) do
        List.iter (fun (v,w) -> Wgraph.add_edge rev_g v u w) (Wgraph.succ g u)
      done;
      rev_g
    )
;;


(* Algorithme de Kosaraju-Sharir pour le calcul des composantes fortement connexes d'un graphe orienté *)
let strongly_connected_components g =
  if (Wgraph.is_directed g = false) then
    (
      Printf.printf "Warning: le graphe donné n'est pas orienté. Inutile d'applique Kosaraju-Sharir\n";
     failwith "Utiliser l'algorithme classique simple de coloration des composantes connexes d'un graphe non orienté\n"
    )
  else (* kosaraju-sharir algorithm *)
  (
    let t = dfs_sort_terminated g in
    let rev_g = reverse g in (* inversion de tous les arcs du graphe *)
    let nv = Wgraph.number_of_vertices rev_g in
    let scc = Array.make nv (-1) in (* tableau des composantes fortement connexes *)
    let rec aux_dfs_rec g u col =
      scc.(u) <- col;
      List.iter (
        fun (v,w) ->
        if scc.(v) = -1 then
          aux_dfs_rec g v col
      ) (Wgraph.succ g u);
    in
  
    let color = ref 0 in
    while (Stack.is_empty t = false) do
      let source = Stack.pop t in
      if (scc.(source) = -1) then
        (aux_dfs_rec rev_g source !color; color := !color + 1)
    done;
    (scc, !color)
  )
;;



strongly_connected_components g2;;
strongly_connected_components g3;; 
strongly_connected_components g4;;


(* 2 SAT et graphe d'implication *)

(* représentation d'une formule sous forme CNF: 
   une CNF est représentée par une liste de clauses (disjonctives)
   chaque clause est une liste d'entiers: chaque entier représente un littéral de la clause. 
   L'entier est positif si le littéral est une variable propositionnelle sans négation, 
   il est négatif si le littéral est la négation d'un variable propositionnelle
 *)

type clause = int list;;
type cnf = clause list;;


let f1 = [ [1]; [2;-1] ];;
let var_max phi =
  let rec var_max_clause c =
    match c with
    |[] -> 0
    |0::q -> failwith "Erreur: la numerotation des variables propositionnelles doit commencer a 1. 0 n'est pas un indice de variable valide"
    |idx_litt::q_clause -> max (abs idx_litt) (var_max_clause q_clause)
  in
  let rec aux phi =
    match phi with
    |[] -> 0
    |c::q_phi -> max (var_max_clause c) (aux q_phi)
  in
  aux phi
;;

var_max f1;;

let f2 = [ [1; 7; -9]; [2;-1] ];;
var_max f2;;

let f3 = [ [1; 7; -9]; [0;-1] ];;
var_max f3;;


let graph_of_2cnf phi =
   
  let rec edges_of_clause c g =

    let pos_idx l = 2*((abs l)-1)
      and
        neg_idx l = 2*((abs l)-1)+1
    in
    
    match c with
    |l1::l2::[] when l1 = -1*l2 -> () (* tiers exclus *)
    |l1::l2::[]  ->
      if l1 > 0 && l2 > 0 then (* (x1 ou x2) (non(x2) -> x1) (non(x1) -> x2) *)
      (
         Wgraph.add_edge g (neg_idx l2) (pos_idx l1) 1.; (*             non(x2) -> x1 *)
         Wgraph.add_edge g (neg_idx l1) (pos_idx l2) 1.  (* contraposée non(x1) -> x2 *)
      )
      else if l1 > 0 && l2 < 0 then (* x1 ou non(x2) *)
      (
         Wgraph.add_edge g (pos_idx l2) (pos_idx l1) 1. ; (*             x2 -> x1 *)
         Wgraph.add_edge g (neg_idx l1) (neg_idx l2) 1.       (* contraposée non(x1) -> non(x2) *)
      )
      else if l1 < 0 && l2 > 0 then (* non(x1) ou x2 *)
      (
         Wgraph.add_edge g (pos_idx l1) (pos_idx l2) 1. ; (*             x1 -> x2 *)
         Wgraph.add_edge g (neg_idx l2) (neg_idx l1) 1.       (* contraposée non(x2) -> non(x1) *)         
      )
      else if l1 < 0 && l2 < 0 then (* (non(x1) ou non(x2) ) (x2 -> non(x1)) (x1 -> non(x2)) *)
      (
        Wgraph.add_edge g (pos_idx l2) (neg_idx l1) 1. ; (*             x2 -> non(x1) *)
        Wgraph.add_edge g (pos_idx l1) (neg_idx l2) 1.   (* contraposée x1 -> non(x2) *)
      )
      else
        failwith "Cas impossible"

    |l1::[] -> edges_of_clause ([l1;l1]) g
    |_ -> List.iter (fun x -> Printf.printf "%d " x) c;  failwith "Cas non traite pour l'instant"
  in
  let rec aux phi g =
  match phi with
  | [] -> g
  | c::q_phi -> edges_of_clause c g; (* ajout des arêtes associées à la clause *)
                aux q_phi g (* appel récursif pour traiter les clauses suivantes *)
  in
  let nv = 2 * (var_max phi) in
  Printf.printf "nv=%d\n" nv;
  let g = Wgraph.init_no_edge nv true in 
  aux phi g
;;


graph_of_2cnf [ [1; 3]; [2;-1] ];;
graph_of_2cnf [ [1]; [2;-1] ];;


let is_2sat phi =
  let g = graph_of_2cnf phi in
  let (scc, nscc ) = strongly_connected_components g in
  let v = ref 0 in
  let is_satisfiable = ref true in
  while (!v < (Array.length scc)-1 && !is_satisfiable = true) do
    if (scc.(!v) = scc.(!v+1) ) then
      is_satisfiable := false;
    v := !v + 2
  done;
  !is_satisfiable
;;

is_2sat [ [1; 3]; [2;-1] ];;
is_2sat [ [1;-2]; [1;3]; [-2]; [1;2]; [2]; [-1;-3]; [1;-3]; [2;3]; [3;-4]; [-1;-4]; [2;-4]; [-3;-4]; [4]];;
