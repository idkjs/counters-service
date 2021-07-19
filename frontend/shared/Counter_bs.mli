(* Auto-generated from "Counter.atd" *)
              [@@@ocaml.warning "-27-32-35-39"]

type id = Counter_t.id

type counter = Counter_t.counter = {
  id: id option;
  name: string;
  value: int
}

type counters = Counter_t.counters = { counters: counter list }

val read_id :  id Atdgen_codec_runtime.Decode.t

val write_id :  id Atdgen_codec_runtime.Encode.t

val read_counter :  counter Atdgen_codec_runtime.Decode.t

val write_counter :  counter Atdgen_codec_runtime.Encode.t

val read_counters :  counters Atdgen_codec_runtime.Decode.t

val write_counters :  counters Atdgen_codec_runtime.Encode.t

