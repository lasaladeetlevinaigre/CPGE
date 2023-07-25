
(*ex 1: syntaxe*)
type flog =
  True
| False
| PropVar of int (* variable propositionnelle, avec son étiquette entière *)
| Not of flog
| And of flog*flog
| Or of flog*flog
| Imply of flog*flog
;;

(* construction de formules logiques *)
let f1 = Or(PropVar(1),PropVar(2));;


(* Affichage infixe de formules logiques *)
let rec flog_to_string f =
    match f with
    | True -> "T"
    | False -> "F"
    | PropVar(i) -> "p"^string_of_int(i)
    | Not(f1) -> "(non "^(flog_to_string f1)^")"
    | And(f1,f2) -> "("^(flog_to_string f1)^" et "^(flog_to_string f2)^")"   
    | Or(f1,f2) -> "("^(flog_to_string f1)^" ou "^(flog_to_string f2)^")"     
    | Imply(f1,f2) -> "("^(flog_to_string f1)^" -> "^(flog_to_string f2)^")"      
;;
let f1 = Or(PropVar(1),PropVar(2));;
flog_to_string f1;;
let f2 = And(Or(PropVar(1),PropVar(2)), PropVar(3));;
flog_to_string f2;;
let f3 = And(Or(PropVar(1),Imply(PropVar(2), Not(PropVar(3)))), PropVar(4));;
flog_to_string f3;;



(* comptage du numéro max de variable propositionnelle impliqué dans une formule logique. *)
(* On initialise le max à -1, car on est certains que les variables sont numérotées, par convention, avec des entiers strictement positifs *)
let flog_maximal_variable_index f =
  let rec aux f acc =
  match f with
  | True | False -> acc
  | PropVar(i) -> max acc i
  | Not(f1) -> aux f1 acc
  | And(f1,f2) | Or(f1,f2) | Imply(f1,f2) -> max (aux f1 acc) (aux f2 acc)
  in aux f (-1)

;;

let f1 = Or(PropVar(1),PropVar(2));;
flog_maximal_variable_index f1;;
let f2 = And(Or(PropVar(1),PropVar(2)), PropVar(7));;
flog_maximal_variable_index f2;;


(*ex 2: évaluation*)

(* evaluation de formules logiques *)
(* la valuation est fournie sous la forme d'un tabeau de booléen, indiquant si la variable propositionnell pi a la valeur true ou false *)
let rec flog_eval f valuation =
  match f with
  | True -> true
  | False -> false
  | PropVar(i) when (1 <= i && i <= (Array.length valuation)) -> valuation.(i-1)
  | Not(f1) -> not (flog_eval f1 valuation)
  | And(f1, f2) -> (flog_eval f1 valuation) && (flog_eval f2 valuation)
  | Or(f1, f2) -> (flog_eval f1 valuation) || (flog_eval f2 valuation)
  | Imply(f1, f2) -> (flog_eval f2 valuation) || (flog_eval (Not(f1)) valuation)
  |_ -> failwith "Invalid logical expression: propositional variable index is probably out of valuation array bounds. Please check that your valuation array as exactly the same number of elements as the number of propositional variables in the logical expression you try to evaluate"
;;

flog_eval f1 [|false;false|];;


(* table de vérite *)



let flog_create_valuations n =
  let nbval = int_of_float(2.**float_of_int(n)) in
  let tab_val = Array.make_matrix nbval n false in
  for i = 0 to (nbval-1) do
    let j = ref i in
    let idx_bit = ref 0 in
    while (!j > 0) do
      (*Printf.printf "%d %d\n" i (!j);*)
      tab_val.(i).(!idx_bit) <- (((!j) mod 2) > 0) ;
      j := (!j)/2;
      idx_bit := !idx_bit + 1
    done; 
  done;
  tab_val
;;

flog_create_valuations 4;;

let flog_table f =
  let n = flog_maximal_variable_index f in
  let tab_val = flog_create_valuations n in
  let nval = (Array.length tab_val) in
  let res = Array.make nval false in
  for i = 0 to (nval-1) do
    let valuation = tab_val.(i) in
    res.(i) <- flog_eval f valuation
  done;
  (tab_val, res)
;;

let flog_print_table f =
  let (valuations, eval) = flog_table f in
  Printf.printf "\nTable de vérité de %s\n" (flog_to_string f);

  if ((Array.length valuations) = 0) then
    failwith "Aucune valuation!!";

  let n = (Array.length valuations.(0)) in
  for j = 0 to (n-1) do
    Printf.printf " p%d |" (j+1)
  done;
  Printf.printf "\n";
  for i = 0 to ((Array.length valuations)-1) do
    let v = valuations.(i) and
        res = eval.(i) in
    for j = 0 to (n-1) do
      if (v.(j) = true) then
        Printf.printf " V  |"
      else
        Printf.printf " F  |"
    done;
    if (res = true) then
      Printf.printf "-----> V\n"
    else
      Printf.printf "-----> F\n"
  done;
;;

let f1 = Or(PropVar(1),PropVar(2));;
flog_print_table f1;;
let (valuations, evaluation) = flog_table f1;;


let f3 = And(Or(PropVar(1),Imply(PropVar(2), Not(PropVar(3)))), PropVar(4));;
flog_print_table f3;;
let (valuations, evaluation) = flog_table f3;;



(*ex 3: substitution et simplification*)
(* substitution *)

let rec flog_substitute f i fsubst  =
    match f with
    | PropVar(k) when k = i -> fsubst
    | True | False | PropVar(_) -> f
    | Not(f1) -> Not(flog_substitute f1 i fsubst)
    | And(f1, f2) -> And (flog_substitute f1 i fsubst, flog_substitute f2 i fsubst)
    | Or(f1, f2) -> Or (flog_substitute f1 i fsubst, flog_substitute f2 i fsubst)
    | Imply(f1, f2) -> Imply (flog_substitute f1 i fsubst, flog_substitute f2 i fsubst)
;;

let f2 = And(Or(PropVar(1),PropVar(2)), PropVar(7));;
flog_substitute f2 2 True;;

(* simplifications *)

let smart_and f1 f2 =
  match (f1, f2) with
  |(True, True) -> True
  |(False, _) -> False
  |(_ , False) -> False
  | _ -> And(f1, f2)
;;

let smart_or f1 f2 =
  match (f1, f2) with
  |(False, False) -> False
  |(True, _) -> True
  |(_ , True) -> True
  | _ -> Or(f1, f2)
;;

let smart_not f1 =
  match f1 with
  | False -> True
  | True -> False
  | Not(f2) -> f2 (* double negation *)
  | _ -> Not(f1)

;;

let smart_imply f1 f2 =
  match (f1, f2) with
  | (False, _) -> True
  | (True, False) -> False
  | (True, True) -> True
  | _ -> Imply(f1, f2)
;;


let rec flog_substitute_and_simplify f i fsubst  =
    match f with
    | PropVar(k) when k = i -> fsubst
    | True | False | PropVar(_) -> f
    | Not(f1) -> smart_not (flog_substitute_and_simplify f1 i fsubst)
    | And(f1, f2) -> smart_and (flog_substitute_and_simplify f1 i fsubst) (flog_substitute_and_simplify f2 i fsubst)
    | Or(f1, f2) -> smart_or (flog_substitute_and_simplify f1 i fsubst) (flog_substitute_and_simplify f2 i fsubst)
    | Imply(f1, f2) -> smart_imply (flog_substitute_and_simplify f1 i fsubst) (flog_substitute_and_simplify f2 i fsubst)
;;


(*ex 4: résolution de SAT(phi) par l'algorithme de Quine *)
let flog_sat_quine_old f =
  
  let rec aux f i n = (* i designe l'indice de la variable propositionnelle qui va être substituée *)
    Printf.printf "Substitution Var(%d) %s\n" i (flog_to_string f);
    if (i > n) then
      failwith "Gros probleme..."
    else
      (
        let fs1 = flog_substitute_and_simplify f i False
        and fs2 = flog_substitute_and_simplify f i True in
        match (fs1, fs2)with
        | (True, _) | (_, True)   -> true
        | (False, False)          -> false
        | (False, _)              -> aux fs2 (i+1) n (* Si on a une valeur fausse dans la branche gauche, on ne poursuit pas dans cette branche et on relance l'algo uniquement sur la branche de droite *)
        | (_, False)              -> aux fs1 (i+1) n (* Si on a une valeur fausse dans la branche droite, on ne poursuit pas dans cette branche et on relance l'algo uniquement sur la branche de gauche *)
        | _                       -> (aux fs1 (i+1) n) || (aux fs2 (i+1) n) (* sinon, on doit poursuivre dans les deux branches *)
      )
  in
  let n = flog_maximal_variable_index f in
  aux f 1 n
;;

let flog_sat_quine f =  
  let rec aux f i n = (* i designe l'indice de la variable propositionnelle qui va être substituée *)
    Printf.printf "Substitution Var(%d) %s\n" i (flog_to_string f);
    if (i > n+1) then
      failwith "Gros probleme..." (* ne doit jamais arriver normalement ...*)
    else
      (
        match f with
        | True   -> true
        | False  -> false
        | _      ->    (let fs1 = flog_substitute_and_simplify f i False in (aux fs1 (i+1) n) )
                    || (let fs2 = flog_substitute_and_simplify f i True  in (aux fs2 (i+1) n) ) (* sinon, on doit poursuivre dans les deux branches *)
      )
  in
  let n = flog_maximal_variable_index f in
  aux f 1 n
;;

let f2 = And(Or(PropVar(1),PropVar(2)), PropVar(3));;
flog_sat_quine_old f2;;
flog_sat_quine f2;;

(* Les deux versions sont bien équivalentes, aucun problème, on fait le même nombre de substitutions et de tests. La premiere version descend un "cran" moins loin dans la récursivté et traite certains cas au niveau du dessus, c'est tout *)


let flog_sat_quine_better f =  
  let rec aux f i n acc lres = (* i designe l'indice de la variable propositionnelle qui va être substituée *)
    (* acc est un accumulateur de type stirng, qui va permettre d'accumuler les différentes étpaes gauche/droite du chemin qui reconstituent la valuation testéemodèle trouvée. lres est un accumulateur de type string list qui va accumuler toutes les valuations modèles trouvées *)
    Printf.printf "Substitution Var(%d) %s\n" i (flog_to_string f);
    if (i > n+1) then
      failwith "Gros probleme..." (* ne doit jamais arriver normalement ...*)
    else
      (
        match f with
        | True   -> acc::lres
        | False  -> lres
        | _      ->    (let fs1 = flog_substitute_and_simplify f i False in (aux fs1 (i+1) n (acc^"0") lres) )
                    @  (let fs2 = flog_substitute_and_simplify f i True  in (aux fs2 (i+1) n (acc^"1") lres) ) (* sinon, on doit poursuivre dans les deux branches *)
      )
  in
  let n = flog_maximal_variable_index f in
  aux f 1 n "" []
;;

(* autre version pour que les valuations soient des chaines de n caractères *)
let flog_sat_quine_better_version2 f =  
  let rec aux f i n acc lres = (* i designe l'indice de la variable propositionnelle qui va être substituée *)
    (* acc est un accumulateur de type stirng, qui va permettre d'accumuler les différentes étpaes gauche/droite du chemin qui reconstituent la valuation testéemodèle trouvée. lres est un accumulateur de type string list qui va accumuler toutes les valuations modèles trouvées *)
    Printf.printf "Substitution Var(%d) %s %s\n" i (flog_to_string f) (Bytes.to_string acc);
    if (i > n+1) then
      failwith "Gros probleme..." (* ne doit jamais arriver normalement ...*)
    else
      (
        match f with
        | True   -> (Bytes.to_string acc)::lres
        | False  -> lres
        | _      ->    (let fs1 = flog_substitute_and_simplify f i False in (Bytes.set acc (i-1) '0'; aux fs1 (i+1) n acc lres) )
                    @  (let fs2 = flog_substitute_and_simplify f i True  in (Bytes.set acc (i-1) '1'; aux fs2 (i+1) n acc lres) ) (* sinon, on doit poursuivre dans les deux branches *)
      )
  in
  let n = flog_maximal_variable_index f in
  aux f 1 n (Bytes.make n '0') []
;;

let fimp = Imply(PropVar(1),PropVar(2));;
flog_sat_quine_better fimp;;
flog_sat_quine_better_version2 fimp;;

let f2 = And(Or(PropVar(1),PropVar(2)), PropVar(3));;
flog_sat_quine_better f2;;
let f3 = And(Or(PropVar(1),Imply(PropVar(2), Not(PropVar(3)))), PropVar(4));;
flog_sat_quine_better f3;;




(*ex 5: mise sous forme DNF ou CNF, ecriture au format DIMACS*)


(* représentation d'une formule sous forme CNF ou DNF *)
type norm = DNF | CNF;;
type clause = int list;;
type flog_norm =
  {
    kind : norm;
    nbvar : int;
    clauses : clause list
  }
;;


let flog_to_dnf f =
  let (valuations, evaluation) = flog_table f in
  let nbval = Array.length valuations in
  if ((Array.length valuations) = 0) then
    failwith "Gros probleme"
  else
  (
    let n = Array.length (valuations.(0)) in
    let list_clauses = ref [] in
    for i = 0 to (nbval-1) do
      let clause = ref [] in
      if (evaluation.(i) = true) then
      (
        for j = 0 to (n-1) do
          if (valuations.(i).(j) = true) then
            clause := (j+1)::(!clause)
          else
            clause := (-1*(j+1))::(!clause)
        done;
        list_clauses := (List.rev (!clause)):: (!list_clauses)
      )
    done;
    {
      kind = DNF;
      nbvar = n;
      clauses = !list_clauses
    }
  )
;;

let f2 = And(Or(PropVar(1),PropVar(2)), PropVar(3));;
flog_to_dnf f2;;


let f3 = And(Or(PropVar(1),Imply(PropVar(2), Not(PropVar(3)))), PropVar(4));;
flog_to_dnf f3;;

let flog_to_cnf f =
  let (valuations, evaluation) = flog_table (Not(f)) in
  let nbval = Array.length valuations in
  if ((Array.length valuations) = 0) then
    failwith "Gros probleme"
  else
  (
    let n = Array.length (valuations.(0)) in
    let list_clauses = ref [] in
    for i = 0 to (nbval-1) do
      let clause = ref [] in
      if (evaluation.(i) = true) then
      (
        for j = 0 to (n-1) do
          if (valuations.(i).(j) = true) then
            clause := (-1*(j+1))::(!clause)
          else
            clause := (j+1)::(!clause)
        done;
        list_clauses := (List.rev (!clause)):: (!list_clauses)
      )
    done;
    {
      kind = CNF;
      nbvar = n;
      clauses = !list_clauses
    }
  )
;;


(* Ecriture au format DIMACS pour compatibilité avec d'autres solvers SAT *)
let flog_write_dimacs f basename =
  let path = ("./"^basename^".dimacs") in  
  try
    let fd = open_out path in
    (
      match f.kind with
      |DNF -> output_string fd ("p dnf "^(string_of_int f.nbvar)^" "^(string_of_int (List.length f.clauses))^"\n")
      |CNF -> output_string fd ("p cnf "^(string_of_int f.nbvar)^" "^(string_of_int (List.length f.clauses))^"\n")
    );
    let rec string_of_clause l =
      let rec aux l acc = 
        match l with
        |[] -> acc^"0\n"
        |litteral::q -> aux q (acc^string_of_int(litteral)^" ")
      in
      aux l ""
    in
    let rec  write_list_of_clauses fd lclauses =
      match lclauses with
      | [] -> ()
      | clause::q -> output_string fd (string_of_clause clause); write_list_of_clauses fd q
    in
    write_list_of_clauses fd (f.clauses);
    close_out fd;
  with
  | e -> Printf.printf "ERREUR\n"; raise e
;;

let f3 = And(Or(PropVar(1),Imply(PropVar(2), Not(PropVar(3)))), PropVar(4));;
let f3_dnf = flog_to_dnf f3;;
flog_write_dimacs f3_dnf;;

let f3_cnf = flog_to_cnf f3;;
flog_write_dimacs f3_cnf;;
