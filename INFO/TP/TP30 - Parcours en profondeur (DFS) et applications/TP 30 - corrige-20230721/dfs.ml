#use "./Wgraph.ml"



(* Construction du graphe g1 *)
let g1 = Wgraph.init_no_edge 8 false;;
Wgraph.add_edge g1 0 1 1.;
Wgraph.add_edge g1 0 2 1.;
Wgraph.add_edge g1 2 1 1.;
Wgraph.add_edge g1 3 5 1.;
Wgraph.add_edge g1 4 5 1.;
Wgraph.add_edge g1 5 7 1.;;


                                                                           
let dfs_rec g source  =
   let nv = Wgraph.number_of_vertices g in
   let visited = Array.make nv false in
   let rec aux_dfs_rec g u =    
     visited.(u) <- true;
     List.iter (fun (v,w) -> if not visited.(v) then
                             (aux_dfs_rec g v)
       ) (Wgraph.succ g u);
      Printf.printf "%d terminé\n" u; 
   in
   aux_dfs_rec g source
;;

(* complexité linéaire en O(nv+ne) car nv = cout de l'initialisation du tableau visited + chaque arc est parcouru au plus une fois*)

(* Complexité spatiale dans le segment de tas en O(nv). 
   Complexité spatiale dans le segment de pile en O(nv) pour un graphe dégénéré de type liste: la pile peut déborder! *)

dfs_rec g1 0;;
dfs_rec g1 7;;

(* dfs graphe non orienté: calcul de composantes connexes *)
let connected_components g =
   if (Wgraph.is_directed g) then
     failwith "La fonction connex_components ne s'applique qu'aux graphes non orientés";

  let nv = Wgraph.number_of_vertices g in
  let cc = Array.make nv (-1) in
  let rec aux_dfs_rec g u col =
    cc.(u) <- col;
    List.iter (
        fun (v,w) ->
        if cc.(v) = -1 then
          aux_dfs_rec g v col
      ) (Wgraph.succ g u);
  in
  
  let color = ref 0 in
  for source = 0 to (nv-1) do
    if (cc.(source) = -1) then
      (aux_dfs_rec g source !color; color := !color + 1)
  done;
  (cc, !color)
;;

let (cc, number_of_cc) = connected_components g1;;


let g2 = Wgraph.init_no_edge 6 true;;
Wgraph.add_edge g2 0 1 1.;
Wgraph.add_edge g2 1 4 1.;
Wgraph.add_edge g2 1 5 1.;
Wgraph.add_edge g2 4 2 1.;
Wgraph.add_edge g2 5 2 1.;
Wgraph.add_edge g2 2 1 1.;
Wgraph.add_edge g2 3 0 1.;
Wgraph.add_edge g2 3 2 1.;;

let (cc, number_of_cc) = connected_components g2;;

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

let dfs_postorder g =
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


let postorder = dfs_postorder g2;;
Stack.iter (fun k -> Printf.printf "%d " k) postorder;;

let tri_topologique = dfs_postorder g3;;
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

let postorder = dfs_postorder arbre;;
Stack.iter (fun k -> Printf.printf "%d " k) postorder;;







(* Algorithme de parcours en profondeur itératif *)



let dfs_iter g source =
  let nv = Wgraph.number_of_vertices g in
  let visited = Array.make nv false in
  let s = Stack.create () in
  let rec aux lsucc =
    match lsucc with
    | [] -> None
    | (v,w)::q  when visited.(v) = false -> Some(v)
    | _::q  ->  aux q
  in
  (    
    Stack.push source s;
    while (not (Stack.is_empty s)) do
      let u = Stack.top s in
      (    
        visited.(u) <- true;
        let l = Wgraph.succ g u in
        match aux l with
        |None -> let terminated = Stack.pop s in ();  Printf.printf "%d terminé\n" terminated;
        |Some(v) -> Stack.push v s
      )
    done
  )
 
;;

dfs_rec g1 0;;
dfs_iter g1 0;;
dfs_rec arbre 0;;
dfs_iter arbre 0;;
dfs_rec g1 7;;
dfs_iter g1 7;;
dfs_rec g4 6;;
dfs_iter g4 6;;
(* pas de différence dans les parcours*)


(* Recherche de cycles dans un graphe *)
type state = Unvisited | InProgress | Terminated;;

let dfs_iter_has_cycle g source =
  
  let nv = Wgraph.number_of_vertices g in
  let visited = Array.make nv Unvisited in
  let s = Stack.create () in
  let has_cycle = ref false in

  let print_cycle s v =
    Printf.printf "Cycle: ";
    let s2 = Stack.copy s in
    let print = ref true in
    while (!print = true) do
      let u = Stack.pop s2 in 
      (if (u = v) then print := false; Printf.printf "%d " u;)
      
    done;
    Printf.printf "\n";
  in
  
  let rec aux lsucc =

    let prv = ref (-1) in
    if (Stack.length s >= 2) then
      (let u = Stack.pop s in (prv := Stack.top s; Stack.push u s));
    
    match lsucc with
    | [] -> None
    | (v,w)::q  when visited.(v) = Unvisited -> Some(v)
    | (v, _)::q when (visited.(v) = InProgress && Wgraph.is_directed g) -> has_cycle := true; print_cycle s v; aux q 
    |(v, _)::q when (visited.(v) = InProgress && (not (Wgraph.is_directed g)) && v <> -1 && v <> !prv)->   has_cycle := true; print_cycle s v; aux q (* pas de cycle de longueur 2 dans un graphe non orienté *)
    | (v,_)::q  -> aux q               
  in
  (    
    Stack.push source s;
    while (not (Stack.is_empty s)) do
      (*Array.iter (fun k -> Printf.printf "%d " k) path;*)
      Printf.printf "\n";
      let u = Stack.top s in
      (    
        visited.(u) <- InProgress;
        let l = Wgraph.succ g u in
        match aux l  with
        |None -> let terminated = Stack.pop s in ();  Printf.printf "\t... %d terminé\n" terminated; visited.(u) <- Terminated;
        |Some(v) -> Stack.push v s; 
      );
    done;
    !has_cycle
  ) 
;;


dfs_iter_has_cycle g1 0;;
dfs_iter_has_cycle arbre 0;;
dfs_iter_has_cycle g3 0;;
dfs_iter_has_cycle g3 5;;
dfs_iter_has_cycle g2 0;;
dfs_iter_has_cycle g4 6;;



