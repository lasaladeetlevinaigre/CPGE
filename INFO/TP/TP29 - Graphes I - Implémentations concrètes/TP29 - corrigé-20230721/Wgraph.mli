module Wgraph :
  sig
    type 'w t

    (* constructeur *)
    val init_no_edge: int -> bool -> 'w t
   
    (* accesseurs *)
    val succ: 'w t -> int -> (int*'w) list
    val print: float t -> unit
    val number_of_vertices : 'w t -> int
    val number_of_edges : 'w t -> int

    (* transformateurs *)
    val add_edge:  'w t -> int -> int -> 'w -> bool
    val remove_edge:  'w t -> int -> int -> bool 

  end
;;





  
