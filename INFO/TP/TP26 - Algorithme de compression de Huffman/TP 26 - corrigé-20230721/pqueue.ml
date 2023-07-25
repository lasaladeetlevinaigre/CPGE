module Pqueue =
  
  struct
    
    type ('k, 'v) pqueue =
      {
        mutable size : int;
        fun_is_priority_greater: 'k -> 'k -> bool;
        capacity : int;
        data : ('k*'v) array (* tableau dont les éléments sont des couples clé-valeur. La clé correspond au degré de priorité *)
      }
    ;;

    (* on donne en entrée: 
  - la capacité max de la file de priorité
  - une couple clé valeur, qui sert a déterminer le type des éléments du tableaux data pour pouvoir allouer le nombre d'octet
  - une fonction de comparaisons sur les clés permettant de savoir ce que signifie "être prioritaire" pour des clés du type choisi *)
    let create capacity_max (kini,vini) f =
      {
        size = 0;
        fun_is_priority_greater = f;
        capacity = capacity_max;
        data = Array.make capacity_max (kini,vini)
      }
    ;;


    let enqueue t (k,v) =
      if (t.size = t.capacity) then
        failwith "Maximal capacity is reached"
      else
        (
          let move_up q i (k,v) =
  
            let idx = ref i in
            let idx_parent = ref ((!idx-1)/2) in
            (
              while (!idx > 0 && (let key = fst q.data.(!idx_parent) in q.fun_is_priority_greater k  key) ) do
                q.data.(!idx) <- q.data.(!idx_parent); (*on décale/descend le parent dans le fils*)
                idx := !idx_parent;
                idx_parent := (!idx-1)/2;
              done;
              q.data.(!idx) <- (k,v)
            )
          in
          move_up t (t.size) (k,v);
          t.size <- t.size + 1
        )
    ;;



    let dequeue q =     
      let move_down q i0 (k,v) =
        
        let i = ref i0 in
        let j = ref (2*i0+1) in
        (
          while ( !j < q.size &&  q.fun_is_priority_greater (fst q.data.(!j)) k  ) do
            if ((!j+1) < q.size && (q.fun_is_priority_greater (fst q.data.(!j+1)) (fst q.data.(!j)) ) ) then
              j := !j+1
            else
              (
                q.data.(!i) <- q.data.(!j);
                i := !j;
                j := 2*(!i) + 1
              )
          done;
          q.data.(!i) <- (k,v);
        )
      in
      (
      if (q.size = 0) then
        failwith "Queue is already empty! Cannot dequeue"
      else
        let top = q.data.(0) in
        (
          q.size <- q.size - 1;
          move_down q 0 (q.data.(q.size));
          top
        )
      )
    ;;
    
    
    let length q =
      q.size
    ;;
    
    let is_empty q =
      (q.size = 0)
    ;;
    
    let is_full q =
      (q.size = q.capacity)
    ;;

    
  end;;


