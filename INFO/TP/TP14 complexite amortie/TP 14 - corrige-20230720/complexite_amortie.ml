type 'a queue =
  {
    rear: 'a list;
    front : 'a list
  };;

let queue_enqueue v q =
  ({rear = v::q.rear; front = q.front}, (List.length q.rear) + 1);;

let queue_dequeue q =
  match q.front with
  | h::t -> ( h, {rear = q.rear; front = t}, List.length q.rear )
  | [] ->
     (
       match List.rev q.rear with
       | [] -> failwith "Empty queue cannot be dequeued"
       | h::t  -> (h, {rear = []; front = t}, 0) (* on vide la queue arriere *)      
     )
;;

let queue_peek q = 
  match q.front with
  | h::t -> h
  | [] ->
     (
       match List.rev q.rear with
       | [] -> failwith "Empty queue cannot be dequeued"
       | h::t  -> h  
     )
;;

let queue_create () =
  {rear = []; front = []}
;;

let q  = queue_create ();;
let (q1, phi1) = queue_enqueue 2 q;;
let (q2, phi2) = queue_enqueue 6 q1;;


let (v, q3, phi3) = queue_dequeue q2;;

let rec successive_enqueue n q0 list_phi =
  match n with
  | 0 -> (q0, list_phi)
  | _ -> let (q, phi) = (queue_enqueue 't'  q0) in  ( successive_enqueue (n-1) q (phi::list_phi) )
;;


let rec successive_dequeue n q0 list_phi =
  match n with
  | 0 -> (q0, list_phi)
  | _ -> let (v, q, phi) = (queue_dequeue q0) in  ( successive_dequeue (n-1) q (phi::list_phi) )
;;

let (q, list_phi) = successive_enqueue 10 (queue_create()) [] in
    let (q, list_phi) = successive_dequeue 5 q list_phi in
    let (q, list_phi) = successive_enqueue 10 q list_phi in
    let (q, list_phi) = successive_dequeue 5 q list_phi in
    let (q, list_phi) = successive_enqueue 10 q list_phi in (List.rev list_phi);;



let rec successive_ops ops q list_phi =
  match ops with
  | [] -> list_phi
  | n::t_ops when n = 0 -> successive_ops t_ops q list_phi
  | n::t_ops when n > 0 -> let (q, list_phi) = (successive_enqueue n q list_phi ) in successive_ops t_ops q list_phi
  | n::t_ops when n < 0 -> let (q, list_phi) = (successive_dequeue (-1*n) q list_phi ) in successive_ops t_ops q list_phi
  |_ -> failwith "bizarre..."
;;

(* test intermediaire *)
let list_ops = [10; -5; 10; -10; 10; -10] in
    let q0 = queue_create () in
    List.rev (successive_ops list_ops q0 []);;

let write_list_csv path l = 
  let rec aux l oc k =
    match l with
    | [] ->  ();
    | v::t -> (output_string oc ((string_of_int k)^","^(string_of_int v)^"\n"); aux t oc (k+1))
  in
  let oc = open_out path in
  aux l oc 0;
  close_out oc
;;
  
let potential list_ops =
  
  let list_phi = (let q0 = queue_create () in
                  List.rev (successive_ops list_ops q0 [])) in
  (write_list_csv "./complexite_amortie.csv" list_phi)
;;


let list_ops = [10; -5; 10; -10; 10; -10] in
potential list_ops;;
