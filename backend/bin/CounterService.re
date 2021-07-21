open Opium;

let ( let* ) = Lwt.bind;

let not_found = Response.make(~status=`Not_found, ()) |> Lwt.return;

let json_from_req = (f, request) => {
  let.await json_string = request.Request.body |> Body.to_string;
  switch (json_string |> Yojson.Safe.from_string |> f) {
  | Ok(data) => Lwt.return(data)
  | Error(error) => Lwt.fail(Invalid_argument(error))
  };
};
let response_from_json = (~status=?, f, data) => {
  let response =
    switch (status) {
    | Some(status) => Response.of_json(~status, f(data))
    | None => Response.of_json(f(data))
    };
  response |> Lwt.return;
};

let read_counter_by_id =
  App.get("/counter/:id", request => {
    let id = Router.param(request, "id");
    let.await found_counter = Storage.read_by_counter_id(~id);
    switch (found_counter) {
    | Some(counter) =>
      let x = counter |> Storage.counter_to_yojson |> Yojson.Safe.to_string;
      Printf.sprintf("found_counter not found: %s", x) |> ignore;
      counter |> response_from_json(Storage.counter_to_yojson);
    | None => not_found
    };
  });

[@deriving yojson]
type create_counter = {
  name: string,
  value: option(int)
};

let post_counter =
  App.post("/counter", request => {
    let.await payload = request |> json_from_req(create_counter_of_yojson);

    let.await counter =
      Storage.create_counter(~name=payload.name,~value=?payload.value,());

    counter |> response_from_json(~status=`Created, Storage.counter_to_yojson);
  });

[@deriving yojson]
type update_payload = {value: int};
let update_counter_by_id =
  App.put("/counter/:id", request => {
    let id = Router.param(request, "id");
    let.await payload = request |> json_from_req(update_payload_of_yojson);
    let.await () = Storage.update_counter(~id, ~value=payload.value);
    Response.make(~status=`No_content, ()) |> Lwt.return;
  });
let increment_counter =
  App.post("/counter/increment/:id", request => {
    let id = Router.param(request, "id");
    let.await () = Storage.increment_counter(~id);
    Response.make(~status=`No_content, ()) |> Lwt.return;
  });
let decrement_counter =
  App.post("/counter/decrement/:id", request => {
    let id = Router.param(request, "id");
    let.await () = Storage.decrement_counter(~id);
    Response.make(~status=`No_content, ()) |> Lwt.return;
  });

let delete_counter_by_id =
  App.delete("/counter/:id", request => {
    let id = Router.param(request, "id");
    let.await () = Storage.delete_counter(~id);
    Response.make(~status=`No_content, ()) |> Lwt.return;
  });
let read_all_counters =
  App.get("/counters", _req => {
    let.await storage = Storage.read_all_counters();
    storage |> response_from_json(Storage.to_yojson);
  });

let health =
  App.get("/", _request => {
    let status = [%yojson {health: true, ok: true}];
    Lwt.return(Response.of_json(status));
  });
