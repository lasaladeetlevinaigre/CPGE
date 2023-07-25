#use "./Wgraph.ml"


let g2 = Wgraph.init_no_edge 6 true;;
Wgraph.add_edge g2 0 1 1.;
Wgraph.add_edge g2 1 5 1.;
Wgraph.add_edge g2 1 4 1.;
Wgraph.add_edge g2 4 2 1.;
Wgraph.add_edge g2 5 2 1.;
Wgraph.add_edge g2 2 1 1.;
Wgraph.add_edge g2 3 2 1.;;
Wgraph.add_edge g2 3 0 1.;



let g3 = Wgraph.init_no_edge 8 true;;
Wgraph.add_edge g3 0 2 1.;
Wgraph.add_edge g3 1 3 1.;
Wgraph.add_edge g3 3 7 1.;
Wgraph.add_edge g3 3 6 1.;
Wgraph.add_edge g3 4 7 1.;
Wgraph.add_edge g3 4 0 1.;
Wgraph.add_edge g3 5 7 1.;
Wgraph.add_edge g3 5 4 1.;
Wgraph.add_edge g3 5 1 1.;
Wgraph.add_edge g3 6 4 1.;
Wgraph.add_edge g3 6 2 1.;
Wgraph.add_edge g3 6 0 1.;
Wgraph.add_edge g3 7 2 1.                                
;;

let g4 = Wgraph.init_no_edge 8 true;;
Wgraph.add_edge g4 1 4 1.0;
Wgraph.add_edge g4 1 3 1.0;
Wgraph.add_edge g4 2 3 1.0;
Wgraph.add_edge g4 3 2 1.0;
Wgraph.add_edge g4 3 0 1.0;
Wgraph.add_edge g4 4 1 1.0;
Wgraph.add_edge g4 4 0 1.0;
Wgraph.add_edge g4 5 7 1.0;
Wgraph.add_edge g4 5 2 1.0;
Wgraph.add_edge g4 6 5 1.0;
Wgraph.add_edge g4 6 2 1.0;
Wgraph.add_edge g4 6 1 1.0;
Wgraph.add_edge g4 7 6 1.0;;


let g5 = Wgraph.init_no_edge 11 true;;
Wgraph.add_edge g5 0 1 1.0;
Wgraph.add_edge g5 1 3 1.0;
Wgraph.add_edge g5 2 0 1.0;
Wgraph.add_edge g5 3 7 1.0;
Wgraph.add_edge g5 3 5 1.0;
Wgraph.add_edge g5 3 4 1.0;
Wgraph.add_edge g5 3 2 1.0;
Wgraph.add_edge g5 4 6 1.0;
Wgraph.add_edge g5 5 4 1.0;
Wgraph.add_edge g5 6 5 1.0;
Wgraph.add_edge g5 7 8 1.0;
Wgraph.add_edge g5 8 9 1.0;
Wgraph.add_edge g5 9 10 1.0;
Wgraph.add_edge g5 9 7 1.0;;

