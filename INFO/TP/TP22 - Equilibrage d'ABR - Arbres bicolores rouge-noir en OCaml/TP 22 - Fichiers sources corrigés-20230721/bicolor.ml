type color = R | B;; (* red or black *)
type 'a bicolor = E | N of color * 'a bicolor * 'a * 'a bicolor;;

let testfig1 = N(B,N(R,N(B,E,1,E),2, N(B,N(R,E,5, E), 7, N(R, E, 8, E))), 11, N(B, E, 14, N(R, E, 15, E)));;

let rec height t =
  match t with
  | E -> -1
  | N(_, l, _, r) -> 1 + ( max (height l) (height r) )
;;

height testfig1;;


(* Comme tous les chemin d'un noeud à une feuille ont le même nombre de noeuds noirs, on prend un seul chemin, n'importe lequel, pour compter, pas de besoin de trouver le chemin max. La complexité est donc en O(h) *)
let rec black_height t =
  match t with
  | E -> 0
  | N(c, l, _, r) when c = B -> 1 + black_height l
  | N(_, l, _, r)->black_height  l
;;

black_height testfig1;;

let is_bst t =
  (* Vérifie que t est un abr et renvoie ses étiquettes minimum et maximum. On
     suppose que t est non vide *)
  let rec aux_is_bst t =
    match t with
    | E -> failwith "Impossible : erreur d'implémentation"
    | N (_, E, v, E) -> (true, v, v) (* cas d'arret *)
    | N (_, E, v, r) ->
       let isbst_r, mini_r, maxi_r = aux_is_bst r in
       (isbst_r && v <= mini_r, v, maxi_r)
    | N (_, l, v, E) ->
       let isbst_l, mini_l, maxi_l = aux_is_bst l in
       (isbst_l && maxi_l <= v, mini_l, v)
    | N (_, l, v, r) ->
       let isbst_r, mini_r, maxi_r = aux_is_bst r in
       let isbst_l, mini_l, maxi_l = aux_is_bst l in
       (isbst_l && isbst_r && maxi_l <= v && v <= mini_r, mini_l, maxi_r)
  in
  match t with
  | E -> true
  | _ -> let (res_is_bst, _, _) = aux_is_bst t in res_is_bst
;;


let rec is_bicolor t =
  (* Vérifie que a est rouge-noir connaissant la couleur de son père
     et renvoie également sa hauteur noire *)
  let rec check t c_parent = 
    match t with
    | E -> (true, 0)
    | N (c, l, _, r) ->
       let ok_l, bh_l = check l c in
       let ok_r, bh_r = check r c in
       ((c_parent = B || c = B) && ok_l && ok_r && bh_l = bh_r),
       (if c = B then 1 else 0) + bh_l
  in
  let ok, _ = check t R (*on doit initialiser à rouge pour s'assurer que la racine est bien noire *)
  in (ok && (is_bst t) )
;;


let testa = N(B,N(B,N(B,E,1,E),2, N(B,N(R,E,5, E), 7, N(R, E, 8, E))), 11, N(B, E, 14, N(R, E, 15, E)));;

let testb = N(R,N(B,N(B,E,1,E),2, N(B,N(R,E,5, E), 7, N(R, E, 8, E))), 11, N(B, E, 14, N(R, E, 15, E)));;

let testc = N(B,N(R,N(B,E,1,E),2, N(R,N(B,E,5, E), 7, N(B, E, 8, E))), 11, N(B, E, 14, N(R, E, 15, E)));;

let testd = N(B,N(R,N(B,E,1,E),2, N(B,N(B,E,0, E), 7, N(R, E, 8, E))), 11, N(B, E, 14, N(R, E, 15, E)));;

is_bicolor testfig1;;

is_bicolor testa;;
is_bst testa;;

is_bicolor testb;;
is_bst testb;;

is_bicolor testc;;
is_bst testc;;

is_bst testd;;
is_bicolor testd;;

let red_corr t =
  match t with
  | N (B, N (R, N (R, a, x, b), y, c), z, d)
  | N (B, N (R, a, x, N (R, b, y, c)), z, d)
  | N (B, a, x, N (R, b, y, N (R, c, z, d)))
  | N (B, a, x, N (R, N (R, b, y, c), z, d))
      -> N (R, N (B, a, x, b), y, N (B, c, z, d))
  | _ -> t
;;


let insert_balance k t =
  let rec insert_and_corr k t =
    match t with
    | E -> N (R, E, k, E)
    | N (c, l, v, r) ->
       if k <= v then
         red_corr ( N(c, (insert_and_corr  k l), v, r) )
       else
         red_corr ( N(c, l, v, (insert_and_corr k r) ) )

  in match insert_and_corr k t with
     | N (_, l, v, r) -> N (B, l, v, r) (* on colorie toujours la racine du sous-arbre en noir *)
    | E -> failwith "impossible"
;;

let test n =
  let build_test_tree n =
    let rec add_from k acc =
      if k > n then acc
      else add_from (k + 1) (insert_balance k acc)
    in
    add_from 1 E
  in
  let tn = build_test_tree n in
  assert (is_bst tn && is_bicolor tn);
  Printf.printf "%d : %d %d\n" n (height tn) (black_height tn)
;;

List.iter test [10; 100; 1_000];;(*; 10_000; 100_000; 1_000_000; 10_000_000];;*)
                                
