#use "pqueue.ml"

let q = Pqueue.create 10 (-1, ' ') (fun n1 n2 -> n1 >= n2);;

Pqueue.enqueue q (2, 'v');;
q;;

Pqueue.enqueue q (7, 'b');;
q;;

Pqueue.enqueue q (1, 'e');;
q;;

let v  = Pqueue.dequeue q;;
let v  = Pqueue.dequeue q;;
Pqueue.length q;;
