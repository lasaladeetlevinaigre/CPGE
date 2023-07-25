let dep=[(-2,-1);(-2,1);(-1,-2);(-1,2);(1,-2);(1,2);(2,-1);(2,1)];;

(* 1. (a) *)
let valide (i,j) = (-1 < i && i < 8) && (-1 < j) && (j < 8);;

(* 1. (b) *)
let successeurs i j =
  let rec aux i j dep acc =
    match dep with
    |[] -> acc
    |((di, dj)::q) -> let suiv = ((i+di),(j+dj)) in
                      if ((valide suiv) = true) then
                        aux i j q (suiv::acc)
                      else
                        aux i j q acc
  in                       
  if (valide(i,j) = false) then
    []
  else
    aux i j dep []
;;

successeurs 3 3;;

(* 1. (c) *)

let flat ll =
  let rec aux ll acc =
    match ll with
    | [] -> acc
    | l::qll -> match l with
                | [] -> aux qll acc
                | h::q -> aux (q::qll) (h::acc)
  in
  List.rev (aux ll []) (* le List.rev sert à coller à l'exmple de l'enonce...mais pas du tout indispensable *)
;;


flat [ []; [(1,1)]; [(1,1);(2,2)]; [(1,1);(2,2);(3,3)] ];;

let liste_successeurs l =
  let rec aux l acc = 
    match l with
    | [] -> acc
    | (i,j)::q -> aux q ((successeurs i j):: acc)
  in
  flat (aux l [])
;;

liste_successeurs [(3,3);(1,1)];;


(* 2 (b) *)

let rec transition l m p =
  let rec aux lsucc m p acc = 
  match lsucc with
  |[] -> acc
  |(i,j)::q when (m.(i).(j) = -1) -> (m.(i).(j) <- p); aux q m p ((i,j)::acc)
  |(i,j)::q -> aux q m p acc (* cas où (i,j) etait déjà accessible *)
  in
  aux l m p []
;;

let m = Array.make_matrix 8 8 (-1);;
transition [(0,0)] m 0;;


(* 3 (c) *)

let cavalier (i,j) =
  if (valide (i,j) = false) then
    failwith "Position invalide"
  else
  let m = Array.make_matrix 8 8 (-1) in
  (
    let rec aux l m p =
      match l with
      |[] -> m
      | _ -> aux (liste_successeurs (transition l m p) ) m (p+1)
    in
    aux [(i,j)] m 0
  )
;;
    
cavalier (0,0);;


(* 3 *)

(* methode naive exhaustive: on teste toutes les positions initiales (i,j). Si dans la matrice, il reste un coefficient (k,l) egal à -1, cela signifie que cette position (k;l) n'est pas accessible depuis (i,j)  *)
let test () =
  let ret = ref true in
  for i = 0 to 7 do
    for j = 0 to 7 do
      let m = cavalier (i,j) in
      for i = 0 to 7 do
        for j = 0 to 7 do
          if (m.(i).(j) = -1) then
            ret := false
          else
            ()
        done
      done
    done
  done;
  !ret
;;

test ();;
(* toute position est accessible à partir de toute position initiale *)

(* methode plus intelligente utilisant la symétrie et la transitivité de la relation (k,l) est accessible depuis (i,j) *)
(* Si on trouve une position (i,j) à partir de laquelle toutes les autres positions (k,l) sont accessibles, alors c'est terminer! 
On va montrer que pour (i,j)=(3,3) par exemple, c'est le cas. On choisit cette position centrale car cela va intuitivement réduire le nombre de tests *)
let test () =
  let ret = ref true in
  let m = cavalier (3,3) in
  let i = ref 0 in
  while (!i < 8 && !ret ) do
    let j = ref 0 in
    while (!j < 8 && !ret ) do
      if (m.(!i).(!j) = -1) then
        ret := false
      else
        j := !j +1;
    done;
    i := !i + 1;
  done;
  !ret
;;
test ();;
