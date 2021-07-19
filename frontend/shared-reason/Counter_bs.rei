/* Auto-generated from "Counter.atd" */
[@ocaml.warning "-27-32-35-39"];

type id = Counter_t.id;

type counter =
  Counter_t.counter = {
    id: option(id),
    name: string,
    value: int,
  };

type counters = Counter_t.counters = {counters: list(counter)};

let read_id: Atdgen_codec_runtime.Decode.t(id);

let write_id: Atdgen_codec_runtime.Encode.t(id);

let read_counter: Atdgen_codec_runtime.Decode.t(counter);

let write_counter: Atdgen_codec_runtime.Encode.t(counter);

let read_counters: Atdgen_codec_runtime.Decode.t(counters);

let write_counters: Atdgen_codec_runtime.Encode.t(counters);
