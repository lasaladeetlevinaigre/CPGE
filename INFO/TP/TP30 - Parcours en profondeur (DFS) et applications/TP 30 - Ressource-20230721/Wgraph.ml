module Wgraph  =

  struct 
    type 'w t =
      {
        num_vertices : int;
        mutable num_edges : int;
        directed : bool;
        tab : ( (int, 'w) Hashtbl.t) array   (* tableau de tables de hachage plutôt que tableau de listes *)
      };;

    (* constructeur *)
    let init_no_edge nv direct =
      let t = Array.make nv (Hashtbl.create 1) in
      (for u = 0 to nv-1 do
        t.(u) <- Hashtbl.create 4
      done;      
      {
        num_vertices = nv;
        num_edges = 0;
        directed = direct;
        tab = t (* on conjecture que le nombre moyen de sommets adjacents à un autre sommet (degré) est autour de 4 *)        
      });;

    (* accesseurs *)
    let succ g u =
      let h = g.tab.(u) in
      let lref = ref [] in (* liste mutable *)
      Hashtbl.iter (fun v p -> lref := (v,p)::!lref) h;
      !lref
    ;;

    let print g =
      for u = 0 to (Array.length g.tab) - 1 do
        let h = g.tab.(u) in
        (
          Printf.printf "Sommet %d:" u;
          Hashtbl.iter (fun v p -> Printf.printf "(%d, %f) " v p) h;
          Printf.printf "\n"
        )
      done
    ;;

    let number_of_vertices g =
      (*let stats = Hashtbl.stats g.h in
      stats.num_bindings*)
      Array.length g.tab
    ;;

    let number_of_edges g =
    (*Hashtbl.fold (fun u lsucc count -> count + (List.length lsucc)) g.h 0*)
      g.num_edges
    ;;

    let is_directed g =
    (*Hashtbl.fold (fun u lsucc count -> count + (List.length lsucc)) g.h 0*)
      g.directed
    ;;
    
    (* transformateurs *)
    let add_edge g u v weight =
      let h = g.tab.(u) in
      let p_opt = Hashtbl.find_opt h v in
      match p_opt with
      | None -> Hashtbl.add g.tab.(u) v weight;
                if (g.directed = false) then
                  Hashtbl.add g.tab.(v) u weight;
                g.num_edges <- g.num_edges + 1;
      | Some(p) ->  Hashtbl.replace g.tab.(u) v weight;
                   if (g.directed = false) then
                     Hashtbl.replace g.tab.(v) u weight;
      
    ;;

    let remove_edge g u v =
      let h = g.tab.(u) in
      let popt = Hashtbl.find_opt h v in
      match popt with
      | None -> false (* l'arete que l'on demande de supprimer n'est pas présente *)
      | Some(p) -> Hashtbl.remove g.tab.(u) v;
                   if (g.directed = false) then
                     Hashtbl.remove g.tab.(v) u;
                   (g.num_edges <- g.num_edges - 1); true;
      
    ;;

  end ;;





  
