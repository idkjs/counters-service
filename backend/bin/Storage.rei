

[@deriving yojson]
type counter = {
  id: string,
  name: string,
  value: int,
};
[@deriving yojson]
type t = list(counter);

let read_all_counters: unit => Lwt.t(list(counter));
let read_by_counter_id: (~id: string) => Lwt.t(option(counter));
// let create_counter: (~name: string) => Lwt.t(counter);
let create_counter: (~name: string, ~value: int=?,unit) => Lwt.t(counter);
let delete_counter: (~id: string) => Lwt.t(unit);
let increment_counter: (~id: string) => Lwt.t(unit);
let decrement_counter: (~id: string) => Lwt.t(unit);
let update_counter: (~id: string, ~value: int) => Lwt.t(unit);
