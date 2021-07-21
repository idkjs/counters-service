
let ( let* ) = Lwt.bind;
[@deriving yojson]
type counter = {
  id: string,
  name: string,
  value: int,
};
[@deriving yojson]
type t = list(counter);

let read_all_counters:unit=>Lwt.t(t) = {
  // open Counter;
  let query = [%rapper
    get_many(
      {sql|
           SELECT @string{id}, @string{name}, @int{value}
          FROM counters
      |sql},
      record_out,
    )
  ];
  () => query() |> Db.dispatch;
};

let read_by_counter_id = {
  // open Counter;
  let query = [%rapper
    get_opt(
      {sql|
        SELECT @string{id}, @string{name}, @int{value}
        FROM counters
        WHERE id = %string{id}
      |sql},
      record_out,
    )
  ];
  (~id) => query(~id) |> Db.dispatch;
};

let create_counter = {
  // open Counter;
  let query = [%rapper
    execute(
      {sql|
        INSERT INTO counters
        VALUES(%string{id}, %string{name}, %int{value})
      |sql},
      record_in,
    )
  ];
  (~name, ~value=0,()) => {
    let id = Uuidm.create(`V4) |> Uuidm.to_string;
    let counter = {id, name, value};
    let.await () = query(counter) |> Db.dispatch;
    Lwt.return(counter);
  };
};


let delete_counter = {
  let query = [%rapper
    execute(
      {sql|
        DELETE FROM counters
        WHERE id = %string{id}
      |sql},
    )
  ];
  (~id) => query(~id) |> Db.dispatch;
};
let update_counter = {
  let query = [%rapper
    execute(
      {sql|
        UPDATE counters
        SET value = %int{value}
        WHERE id = %string{id}
      |sql},
    )
  ];
  (~id, ~value) => query(~id, ~value) |> Db.dispatch;
};
let increment_counter = {
  let query = [%rapper
    execute(
      {sql|
        UPDATE counters
        SET value = value + 1
        WHERE id = %string{id}
      |sql},
    )
  ];
  (~id) => query(~id) |> Db.dispatch;
};
let decrement_counter = {
  let query = [%rapper
    execute(
      {sql|
        UPDATE counters
        SET value = value - 1
        WHERE id = %string{id}
      |sql},
    )
  ];
  (~id) => query(~id) |> Db.dispatch;
};

