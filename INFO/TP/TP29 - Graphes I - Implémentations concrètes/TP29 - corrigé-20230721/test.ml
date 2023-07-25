#use "./Wgraph.ml"

(* TEST 1: DIRECTED GRAPH *)
let g  = Wgraph.init_no_edge 6 true;;

Wgraph.add_edge g 0 1 1.;;
Wgraph.add_edge g 1 4 2.;;
Wgraph.add_edge g 1 5 3.;;
Wgraph.add_edge g 4 2 4.;;
Wgraph.add_edge g 5 2 5.;;
Wgraph.add_edge g 2 1 6.;;
Wgraph.add_edge g 3 2 7.;;
Wgraph.add_edge g 3 0 1.;;

Wgraph.number_of_vertices g;;
Wgraph.number_of_edges g;;

Wgraph.print g;;

Wgraph.remove_edge g 4 2;;

Wgraph.number_of_edges g;;
Wgraph.print g;;



(* TEST 2: NON-DIRECTED GRAPH *)
let g  = Wgraph.init_no_edge 9 false;;

Wgraph.add_edge g 1 3 1.;;
Wgraph.add_edge g 3 7 1.;;
Wgraph.add_edge g 3 6 1.;;
Wgraph.add_edge g 6 2 1.;;
Wgraph.add_edge g 7 2 1.;;
Wgraph.add_edge g 6 0 1.;;
Wgraph.add_edge g 0 2 1.;;
Wgraph.add_edge g 6 4 1.;;
Wgraph.add_edge g 4 7 1.;;
Wgraph.add_edge g 4 0 1.;;
Wgraph.add_edge g 5 1 1.;;
Wgraph.add_edge g 5 7 1.;;
Wgraph.add_edge g 5 4 1.;;

Wgraph.number_of_vertices g;;
Wgraph.number_of_edges g;;

Wgraph.print g;;

Wgraph.remove_edge g 5 7;;
Wgraph.remove_edge g 5 6;;

Wgraph.number_of_edges g;;

Wgraph.print g;;

