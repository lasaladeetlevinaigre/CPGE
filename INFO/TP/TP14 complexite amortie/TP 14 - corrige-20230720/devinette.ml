
let rec aux_devinette n nb_try =
     let () = print_string "taper un nombre : " in
     let i = read_int () 
     in 
       if i = n then 
         let () = print_string ("BRAVO! Vous avez réussi en "^(string_of_int nb_try)^" essais") in
         let () = print_newline ()
         in print_newline ()
       else 
         let () = 
           if i < n then 
             let () = print_string "C'est plus"
             in print_newline () 
           else 
             let () = print_string "C'est moins"
             in print_newline ()
         in aux_devinette n (nb_try+1)
;;


let devinette = 
  let () = Random.self_init() in
  let n = Random.int(100) in  aux_devinette n 1
;;
