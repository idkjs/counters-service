// open Opium;
let ( let* ) = Lwt.bind;

exception Query_failed(string);
let connection_uri = "postgresql://postgres:password@localhost:5432";
let pool =
  switch (
    Caqti_lwt.connect_pool(~max_size=10, Uri.of_string(connection_uri))
  ) {
  | Ok(pool) => pool
  | Error(err) => failwith(Caqti_error.show(err))
  };

let migrate_counters = [%rapper
  execute(
    {|
        CREATE TABLE IF NOT EXISTS counters (
        id    TEXT PRIMARY KEY  DEFAULT uuid_generate_v4(),
        name  TEXT,
        value INTEGER DEFAULT 0
      );
      |},
  )
];
let dispatch = f => {
  let* result = Caqti_lwt.Pool.use(f, pool);
  switch (result) {
  | Ok(data) => Lwt.return(data)
  | Error(error) => Lwt.fail(Query_failed(Caqti_error.show(error)))
  };
};
let () = {
  Caqti_lwt.Pool.use(migrate_counters(), pool)
  |> Lwt_main.run
  |> (
    fun
    | Ok(_) => ()
    | Error(error) => {
        failwith(Caqti_error.show(error));
      }
  );
};
/* Establishes a live database connection and is a pool of
   concurrent threads for accessing that connection. */
let connect = () => {
  Logs.info(m => m("Database URL %s", connection_uri));
  let uri = connection_uri |> Uri.of_string;

  switch (Caqti_lwt.connect_pool(~max_size=10, uri)) {
  | Ok(pool) => {
    Logs.info(m => m("Database connected"));
    pool;
  }
  | Error(err) => failwith(Caqti_error.show(err))
  }
};

/* Type aliases for the sake of documentation and explication */
type caqti_conn_pool('err) =
  Caqti_lwt.Pool.t(Caqti_lwt.connection, [> Caqti_error.connect] as 'err);

type query('res, 'err) =
  Caqti_lwt.connection => Lwt.t(result('res, [< Caqti_error.t] as 'err))
/* [query_pool query pool] is the [Ok res] of the [res] obtained by executing
   the database [query], or else the [Error err] reporting the error causing
   the query to fail. */
let query_pool = (query, pool) =>
  Caqti_lwt.Pool.use(query, pool) |> Lwt_result.map_err(Caqti_error.show);

/* Seal the key type with a non-exported type, so the pool cannot be retrieved
   outside of this module */
type db_pool('err) = caqti_conn_pool('err);

let key: Opium.Context.key(db_pool(Caqti_error.t)) =
  Opium.Context.Key.create(("db pool", _ => Core.Sexp.of_string("db_pool")));
/* Initiate a connection pool and add it to the app environment */
let middleware = app => {
  let pool = connect();
  let filter = (handler, request: Opium.Request.t) => {
    let env = Opium.Context.add(key, pool, request.Opium.Request.env);
    handler({...request, env});
  };

  let middleware = Rock.Middleware.create(~name="DB connection pool", ~filter);
  Opium.App.middleware(middleware, app);
};

