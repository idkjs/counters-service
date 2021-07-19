open Opium;

open CounterService;
let port = Lib.Env.get("PORT") |> int_of_string;
let application =
  App.empty
  |> health
  |> read_counter_by_id
  |> update_counter_by_id
  |> read_all_counters
  |> post_counter
  |> increment_counter
  |> decrement_counter
  |> delete_counter_by_id
  |> Db.middleware
  |> App.port(port)
  |> App.middleware(Middleware.logger)
  |> App.middleware(
       Middleware.allow_cors(~origins=["*"], ~expose=["*"], ()),
     )
  |> App.cmd_name("Counter as a Service");

Logs.set_reporter(Logs_fmt.reporter());
Logs.set_level(Some(Logs.Info));

switch (App.run_command'(application)) {
| `Ok(app) => Lwt_main.run(app)
| `Error => exit(1)
| `Not_running => exit(0)
};
