let baseUrl = "http://127.0.0.1:3003/";
let countersUrl = baseUrl ++ "counters";
let counterUrl = baseUrl ++ "counter";
let deleteUrl = baseUrl ++ "counter/";
let incrUrl = baseUrl ++ "counter/increment/";
let decrUrl = baseUrl ++ "counter/decrement/";
// let create_counter_post =(id)=> baseUrl ++ "counter/" ++ id;
// /counter/decrement/

type t = RemoteData.t(Counters.Counter_t.counters);

exception DecodeError(string);
exception ServerError(string);
  // |> read_counter_by_id
  // |> update_counter_by_id
  // |> read_all_counters
  // |> post_counter
  // |> increment_counter
  // |> delete_counter_by_id
let handleCounterData = data => {
  /* This has to be wrong. Will get back to it */
  let dict = Js.Dict.empty();
  Js.Dict.set(dict, "counters", data);
  let json: Js.Json.t = Js.Json.object_(dict);
  /* turn it into a proper record */
  let counters: Counter_t.counters =
    Atdgen_codec_runtime.Decode.decode(Counter_bs.read_counters, json);

  counters;
};
let fetchCounters = () =>
  Js.Promise.(
    Fetch.fetch(countersUrl)
    |> then_(res => Fetch.Response.json(res))
    |> then_(json => json |> handleCounterData |> resolve)
  );
let doFetch = (method) =>
  Fetch.RequestInit.make(
    ~method_=method,
    ~headers=Fetch.HeadersInit.make({"Content-Type": "application/json"}),
    (),
  );

let post = doFetch(Post);
let delete = doFetch(Delete);
let incrementCounter = (~id) =>
  Js.Promise.(
    Fetch.fetchWithInit(incrUrl ++ id,post)
    |> resolve
    // |> then_(json => json |> handleCounterData |> resolve)
  );
let decrementCounter = (~id) =>
  Js.Promise.(
    Fetch.fetchWithInit(decrUrl ++ id,post)
    |> resolve
  );
let createCounter = (~name) =>{
  let payload = Js.Dict.empty();
  Js.Dict.set(payload, "name", Js.Json.string(name));
  Js.Dict.set(payload, "value", Js.Json.number(0.));
  Js.log(payload)
  Js.Promise.(
    Fetch.fetchWithInit(
      counterUrl,
      Fetch.RequestInit.make(
        ~method_=Post,
        ~body=Fetch.BodyInit.make(Js.Json.stringify(Js.Json.object_(payload))),
        ~headers=Fetch.HeadersInit.make({"Content-Type": "application/json"}),
        ()
      )
    )
    |> then_(Fetch.Response.json)
  );
};

let deleteCounter = (~id) =>
  Js.Promise.(
    Fetch.fetchWithInit(deleteUrl ++ id,delete)
    |> resolve
  );
