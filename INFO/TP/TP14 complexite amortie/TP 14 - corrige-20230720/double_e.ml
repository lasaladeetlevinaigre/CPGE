let count_e s =
  let n = String.length s in
  let cpt = ref 0 in
  for i=0 to (n-1) do
    if (s.[i] = 'e') then
      cpt := !cpt + 1
    else
      ()
  done;
  !cpt
;;

let double_e s =
  let n = String.length s in
  let ne = count_e s in
  let new_n = n + ne in
  let new_s = String.make new_n 'a' in
  let j = ref 0 in
  for i=0 to (n-1) do
    if (s.[i] = 'e') then
    (
      new_s.[!j] <- 'e';
      new_s.[!j+1] <- 'e';
      j := !j + 2
    )
    else
    (
      new_s.[!j] <- s.[i];
       j := !j + 1
    )
    
  done;
  new_s
;;

double_e "je t'aime";;

let double_e_file in_path out_path =
  
  let rec aux ic oc =
    let s = input_line ic in
    let () = output_string oc (double_e s) in
    aux ic oc
  in
  let ic = open_in in_path in
  let oc = open_out out_path in
  try
    aux ic oc
  with
  |Sys_error s -> failwith ("Erreur systeme: "^s)
  |End_of_file -> close_in ic; close_out oc
;;



let rec test list_bench =
  match list_bench with
  | [] -> ()
  | path::t -> double_e_file ("./bench/"^path) ("./test_"^path); test t
;;

let bench = ["simple.txt"; "exemple.txt"; "sample1.txt"; "sample2.txt"; "sample3.txt"; "sample4.txt"; "knuth.txt" ];;

(* on lance les tests automatiques sur toute les cas tests = benchmark *)
test bench;;


(* pour le fichier test simple.txt, l'algorithme ne marche pas bien car les retours à la ligne ne sont pas bien gérés!
Solution: utiliser input_char, mais elle est hors programme... *)

