#use "pqueue.ml"

(* Huffman*)

(* On cree un tableau associatif tab utilisant le mapping  char -> int associant de maniere bijective un caractère ASCII étendu et son code ascii (de 0 à 255) *)
let huffman_count_occurrences text =
  let tab = Array.make 256 0 in
  let ntot = ref 0 in
  (* def d'une fonction interne, qui incremente le nombre d'occurrence d'un caractere c dans le tableau associatif tab *)
  let add_occurrence c =
    let code_ascii = Char.code c in
    (
      tab.(code_ascii) <- tab.(code_ascii) + 1;
      ntot := !ntot + 1
    )
  in
  (* application de cette fonction a tous les caracteres de la chaine text *)
  String.iter add_occurrence text;
  (!ntot, tab)
;;

(* version alternative, avec une table de hashage... mais pas super pertinent ici, car un tableau associatif est possible et même moins gourmand en terme de complexite spatiale *)
(*
let huffman_count_occurrences text =
  let ht = Hashtbl.create 256 in
  let ntot = ref 0 in 
  for i=0 to ((String.length text) -1) do
    ntot := !ntot + 1;
    let c = text.[i] in
    let nocc = Hashtbl.find_opt ht c in
    match nocc with
      |None    -> Hashtbl.add ht c 1
      |Some(n) -> Hashtbl.replace ht c (n+1)
  done;
  (!ntot, ht)
;;

let print_occurrences ht =
  Hashtbl.iter (fun c n -> Printf.printf "%c:%d\n" c n) ht
;;
*)


let test1 = "scienceinformatique";;
let tab1 = huffman_count_occurrences test1;;
let test2 = "abracadabra";;
let tab2 = huffman_count_occurrences test2;;


type hufftree = L of char | N of hufftree*hufftree;;

let huffman_build_tree text =

  let (nb_char, tab_occ) = huffman_count_occurrences text in

  (* allocation d'une structure la file de priorite *)
  (* les éléments de cette file sont des couples poids(entier), arbre de huffman *)
  let q = Pqueue.create nb_char (-1, L('a')) (fun p1 p2 -> p1 <= p2) in

  (* Pour tout caractere dont le nombre d'occurences dans text n'est pas nul, ajouter (p, L(c)) dans la file de priorité, où L(c) est un arbre feuille étiqueté par le caractère c et p le poids de cet arbre, donc ici, comme il s'agit d'arbre feuille, le nombre d'occurrences de ce caractère*)
  for i = 0 to (nb_char -1) do
    let nbocc = tab_occ.(i) in
    if (nbocc <> 0) then
      (
        let c = Char.chr i in 
        let leaf = L(c) in (* construction de l'abre feuille *)
        Pqueue.enqueue q  (nbocc, leaf)
      )
  done;

    
  while (Pqueue.length q >= 2) do
    let (p1, node1) = Pqueue.dequeue q and
        (p2, node2) = Pqueue.dequeue q 
    in
    let newnode = N(node1, node2) in  Pqueue.enqueue q (p1+p2, newnode)
  done;
  Pqueue.dequeue q
;;

(* version avec Array.iteri *)
let huffman_build_tree text =

  let (nb_char, tab_occ) = huffman_count_occurrences text in
  let q = Pqueue.create nb_char (-1, L('a')) (fun p1 p2 -> p1 <= p2) in 
  let enqueue_leaf i nbocc =
    if (nbocc <> 0) then
      let c = Char.chr i in 
      let leaf = L(c) in
      Pqueue.enqueue q  (nbocc, leaf)
    else
      ()
  in
  Array.iteri enqueue_leaf tab_occ;
  while (Pqueue.length q >= 2) do
    let (p1, node1) = Pqueue.dequeue q and
        (p2, node2) = Pqueue.dequeue q 
    in
    let newnode = N(node1, node2) in  Pqueue.enqueue q (p1+p2, newnode)
  done;
  Pqueue.dequeue q
;;
(*;;*)


let texte = "scienceinformatique";;
let texte = "abracadabra";;
huffman_build_tree texte;;


let huffman_build_encoding_table text =
  (* creation de l'arbre de Huffman *)
  let (nb_char, t) = huffman_build_tree text in
  (* generation de la table d'encodage --> prefixes *)
  let encoding_table = Hashtbl.create nb_char in
  let rec aux t enc =
    match t with
    |L(c) -> Hashtbl.add encoding_table c enc
    |N(l,r) -> aux l (enc^"0"); aux r (enc^"1")
  in
  (
    match t with
    |L(c) -> Hashtbl.add encoding_table c "0" (* gestion du cas tres particulier où la chaine ne contient qu'un unique caractere *)
    | _ -> aux t ""
  );
  (t, encoding_table)

;;

let texte = "abracadabra";;
let (t,enc_tab) = huffman_build_encoding_table texte;;

let print_encoding_table h =
  Hashtbl.iter (fun c s -> Printf.printf "%c:%s\n" c s) h
;;

print_encoding_table enc_tab;;

let texte = "Je m'appelle Nicolas";;
let (t1,enc_tab1) = huffman_build_encoding_table texte;;
print_encoding_table enc_tab1;;





let huffman_compress_old text =
  let (t, enc_tab) = huffman_build_encoding_table text in
  let len = String.length text in
  let res = ref "" in
  for i=0 to (len-1) do
    let c = text.[i] in
    let code_c = Hashtbl.find enc_tab c in
    res := (!res)^code_c
  done;
  let compression_rate = (float_of_int (String.length(text)*8)) /. (float_of_int (String.length (!res) ) )  in

  (!res, t, compression_rate)
;;

(* Fonction amelioree, utilisant String.iter (pour un code plus compact) et, surtout, le module Buffer permettant de manipuler de maniere efficace des chaines de caracteres mutables et de longueur variable *)
let huffman_compress text =
  let (t, enc_tab) = huffman_build_encoding_table text in
  let buf = Buffer.create (String.length text) 
  in String.iter
       (fun c ->  let code_c = Hashtbl.find enc_tab c in Printf.printf "Compression: Find %c %s\n" c code_c;
                  Buffer.add_string buf code_c)
       text;
  let compression_rate = (float_of_int (String.length(text)*8)) /. (float_of_int (Buffer.length buf ) )  in

  (Buffer.contents buf, t, compression_rate)
;;

let text = "abracadabra";;
let (encoded_text, huffman_tree, compression_rate) = huffman_compress text;;
Printf.printf "Taille de la chaine originale: %d bits\n" ((String.length text)*8);;
Printf.printf "Taille de la chaine compressee: %d bits\n" (String.length encoded_text);;
Printf.printf "On a bien un taux de compression de %f (réduction de la taille mémoire d'un facteur %f, sans compter le stockage de l'arbre de Huffman...) \n" compression_rate compression_rate;;




let huffman_uncompress_old encoded_text huffman_tree =
  let rec aux encoded_text t i len acc =
    Printf.printf "%d: acc=%s len=%d\n" i acc len;
    match (t, i) with
    | (_, i) when (i=len) -> acc (* toute la chaine compressee a été traitée : fin *)
    | (N(L(c),_), _) when (encoded_text.[i] = '0') -> Printf.printf "Caractere %c decode!\n" c; aux encoded_text huffman_tree (i+1) len (acc^(String.make 1 c)) (* on a atteint une feuille en allant à gauche: on a decodé un code correspondant à un caractère. On continue (appel récursif) à analyser la chaine compressée (i <- i+1) en repartant au début de l'arbre huffman_tree,  après avoir accumulé le caractère que l'on vient de décoder dans la chaine decompressee qui sera notre resultat final acc^string_of_char(c)*)
    | (N(_,L(c)), _) when (encoded_text.[i] = '1') -> Printf.printf "Caractere %c decode!\n" c; aux encoded_text huffman_tree (i+1) len (acc^(String.make 1 c))
    | (N(l,r),_) when (encoded_text.[i] = '0')-> aux encoded_text l (i+1) len acc
    | (N(l,r),_) when (encoded_text.[i] = '1')-> aux encoded_text r (i+1) len acc
    |_ -> failwith "pb"
  in
  aux encoded_text huffman_tree 0 (String.length encoded_text) ""
;;

let text = "abracadabra";;
let (encoded_text, huffman_tree, compression_rate) = huffman_compress text;;
huffman_uncompress encoded_texte huffman_tree;;

let huffman_uncompress encoded_text huffman_tree =
  let rec aux encoded_text t i len acc =
    Printf.printf "%d: acc=%s len=%d\n" i (Buffer.contents acc) len;
    match (t, i) with
    | (_, i) when (i=len) -> acc (* toute la chaine compressee a été traitée : fin *)
    | (N(L(c),_), _) when (encoded_text.[i] = '0') -> Printf.printf "Caractere %c decode!\n" c; aux encoded_text huffman_tree (i+1) len (Buffer.add_char acc c; acc) (* on a atteint une feuille en allant à gauche: on a decodé un code correspondant à un caractère. On continue (appel récursif) à analyser la chaine compressée (i <- i+1) en repartant au début de l'arbre huffman_tree,  après avoir accumulé le caractère que l'on vient de décoder dans la chaine decompressee qui sera notre resultat final acc^string_of_char(c)*)
    | (N(_,L(c)), _) when (encoded_text.[i] = '1') -> Printf.printf "Caractere %c decode!\n" c; aux encoded_text huffman_tree (i+1) len (Buffer.add_char acc c; acc) 
    | (N(l,r),_) when (encoded_text.[i] = '0')-> aux encoded_text l (i+1) len acc
    | (N(l,r),_) when (encoded_text.[i] = '1')-> aux encoded_text r (i+1) len acc
    |_ -> failwith "pb"
  in
  let n = (String.length encoded_text) in
  let buf = aux encoded_text huffman_tree 0 n (Buffer.create (5*n) ) (* On alloue un tableau de taille 5 fois la taille de la chaine compressee, en esperant que notre compression a ete performante, aevc un taux de compression de l'ordre de 5! *)
  in Buffer.contents buf
;;

let text1 = "abracadabra";;
let (t1,enc_tab1) = huffman_build_encoding_table text1;;
print_encoding_table enc_tab1;;
let (encoded_text1, huffman_tree1, compression_rate1) = huffman_compress text1;;
huffman_uncompress encoded_text1 huffman_tree1;;

(* Test permettant de prouver que ce cas tres particulier est également géré *)
let text2 = "tttttttttttttttttttttttttt";;
let (t2,enc_tab2) = huffman_build_encoding_table text2;;
print_encoding_table enc_tab2;;
let (encoded_text2, huffman_tree2, compression_rate2) = huffman_compress text2;;
