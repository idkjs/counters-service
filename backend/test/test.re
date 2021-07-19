open Stdio;
open Counteratdgen;
module Add = {
  type t = {
    name: string,
    value: int,
  };
// this might be redundant since you get a Type_t anyway when you add an id to Add.t
  let encode = t => Types_j.t_of_string(t);
  let decode = t => Types_j.string_of_t(t);
};

let genId = () => Uuidm.v(`V4) |> Uuidm.to_string;
let make = (c: Add.t): Types_t.t => {
  name: c.name,
  value: c.value,
  id: Some(genId()),
};
let makeCounter1 = () => {
  let counter1 = make({name: "counter1", value: 1});
  let counter2 = make({name: "counter2", value: 2});
  // let x = counter2|>Types_j.(buf)

  let buf: Types_t.counters = {counters: [counter1, counter2]};
  // counters to json
  let counters = Types_j.string_of_counters(buf);
  // json counters to Types_t.counters
  let counters_j:Types_t.counters = Types_j.counters_of_string(counters);
  // let counter = counters.counters |> List.hd;
};
let parseCounter1 = () => {
  let buf = In_channel.read_all("resources/counters.json");
  let counters = Types_j.counters_of_string(buf);
  let counter = counters.counters |> List.hd;

  switch (counter.id) {
  | Some(id) =>
    Alcotest.(check(string))(
      "Parse id 1",
      "7d901778-de83-42bd-8824-3ecb7a14d584",
      id,
    )
  | None => ()
  };
};

let () =
  Alcotest.run(
    "Atgen-example",
    [("AtdgenTests", [("counters_decoding", `Slow, parseCounter1)])],
  );
