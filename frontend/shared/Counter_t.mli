(* Auto-generated from "Counter.atd" *)
              [@@@ocaml.warning "-27-32-35-39"]

type id = Transform.Id.t

type counter = { id: id option; name: string; value: int }

type counters = { counters: counter list }
