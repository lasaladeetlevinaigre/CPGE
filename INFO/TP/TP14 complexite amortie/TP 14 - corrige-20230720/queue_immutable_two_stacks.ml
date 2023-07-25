

type 'a queue =
  {
    rear: 'a list;
    front : 'a list
  };;

let queue_enqueue v q =
  {rear = v::q.rear; front = q.front} ;;

let queue_dequeue q =
  match q.front with
  | h::t -> ( h, {rear = q.rear; front = t} )
  | [] ->
     (
       match List.rev q.rear with
       | [] -> failwith "Empty queue cannot be dequeued"
       | h::t  -> (h, {rear = []; front = t})       
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
let q1 = queue_enqueue 2 q;;
let q2 = queue_enqueue 6 q1;;
q2;;

let (v, q3) = queue_dequeue q2;;
