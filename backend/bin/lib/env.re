exception EnvVarNotFound(string);

let get = (name) => {
  switch (Sys.getenv_opt(name)) {
    | Some(value) => value
    | None => {
      failwith("Enviroment variable '" ++ name ++ "' hasn't been found");
    }
  }
}
