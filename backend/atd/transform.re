module Id = {
  type t = string

  let wrap = Fun.id;

  let unwrap = Fun.id;

  let to_string = unwrap;
};
