open UI;
let listToReactArray = items => items->Belt.List.toArray->React.array;

module CreateCounter = {
  [@react.component]
let make = () => {
  let name = () => BsFaker.Random.words(~count=2, ());
  <div>
    <button onClick={_ => Api.createCounter(~name=name())->ignore}>
      {React.string("CreateCounter")}
    </button>
  </div>;
};
}
module H1 = [%styled.h1
  {|
  font-size: 32px;
  font-weight: 700;
  margin: 0;
|}
];

module H2 = [%styled.h2
  {|
  font-size: 25px;
  font-weight: 600;
  margin: 0;
|}
];

module Layout = [%styled.div
  {|
  width: 100%;
  max-width: 840px;
  padding: 80px 28px;
|}
];

module Card = [%styled.div
  (~background="white") => [|
    CssJs.unsafe("backgroundColor", background),
    [%css "border: 2px solid rgba(0 0 0 / 30%)"],
    [%css "border-radius: 6px"],
    [%css "width: 100%"],
    [%css "padding: 28px 40px"],
  |]
];

module Button = [%styled.button
  (~size_=`M, ~kind_=`Primary, ~fullWidth=false) => {
    [|
      fullWidth ? [%css "width: 100%"] : [%css "width: auto"],
      [%css "position: relative"],
      [%css "font: inherit"],
      [%css "color: inherit"],
      [%css "appearance: none"],
      [%css "cursor: pointer"],
      [%css "display: flex"],
      [%css "align-items: center"],
      [%css "justify-content: center"],
      [%css "text-decoration: none"],
      [%css "border-radius: 6px"],
      switch (size_) {
      | `S =>
        %css
        "padding: 2px 4px"
      | `M =>
        %css
        "padding: 8px 12px"
      | `L =>
        %css
        "padding: 14px 22px"
      },
      switch (kind_) {
      | `Primary =>
        %css
        "background: rgb(0 0 0 / 40%)"
      },
    |];
  }
];

module Input = [%styled.input
  [|[%css "font: inherit"], [%css "color: inherit"], [%css "width: 100%"]|]
];

module CounterItem = {
  [@react.component]
  let make = (~name, ~value, ~id: option(Counters.Counter_t.id)) => {
    let id =
      switch (id) {
      | Some(id) => id
      | None => failwith("id is required")
      };
    let handleDelete = event => {
      Js.log("deleting");
      ReactEvent.Mouse.preventDefault(event);
      Api.deleteCounter(~id)->ignore;
    };
    let handleIncr = event => {
      ReactEvent.Mouse.preventDefault(event);
      Js.log("incrementing");
      Api.incrementCounter(~id)->ignore;
    };
    let handleDecr = event => {
      Js.log("decrementing");

      ReactEvent.Mouse.preventDefault(event);
      Api.decrementCounter(~id)->ignore;
    };
    <Card>
      <Row distribute=`Between gap=2>
        <Text size=`L align=`Left> name </Text>
        <Row fullWidth=false gap=3>
          <Text size=`L align=`Left weight=`Bold>
            {value |> string_of_int}
          </Text>
          <Button size_=`M onClick=handleIncr>
            <Text size=`L weight=`Bold> "+" </Text>
          </Button>
          <Button size_=`M onClick=handleDecr>
            <Text size=`L weight=`Bold> "-" </Text>
          </Button>
          <Button size_=`M onClick=handleDelete>
            <Text size=`L weight=`Bold> "x" </Text>
          </Button>
        </Row>
      </Row>
    </Card>;
  };
};
module Encode = {
  let encodeName = (name): Js.Json.t => {
    Atdgen_codec_runtime.Encode.encode(
      Counter_bs.write_counter,
      name,
      // open Json.Encode;
      // object_(list{
      //   ("todo", string(name))
      // })
    );
  };
};
module NewCounter = {
  [@react.component]
  let make = () => {
    let (visible, setVisibility) = React.useState(_ => false);
    let (name, setName) = React.useState(() => "");

    let open_ = () => setVisibility(_ => true);
    let close = () => setVisibility(_ => false);
    let handleClick = _event => {
      open_();
    };

    let handleBackdropClick = event => {
      let value = event->ReactEvent.Mouse.target##value;
      Js.log(value);
      // close();
    };

    let onChange = e => {
      ReactEvent.Form.preventDefault(e);
      let value = e->ReactEvent.Form.target##value;
      Js.log(value);
      setName(_prev => value);
    };

    let onSubmit = e => {
      ReactEvent.Form.preventDefault(e);

      // let body =
      //   name |> Js.Json.stringify(Encode.encodeName) -> Fetch.BodyInit.make;
      // Fetch.BodyInit.make(Js.Json.stringify(Js.Json.object_(payload)));
      // let headers =
      //   Fetch.HeadersInit.make({"Content-Type": "application/json"});

      let _ =
        Api.createCounter(~name)
        // ->Js.Promise.then_(Fetch.Response.json, _)
        ->Js.Promise.then_(response => response->Js.Promise.resolve, _)
        ->Js.Promise.then_(data => data->Js.Promise.resolve, _);

      setName(_prev => "");
      close();
    };
    <>
      <Button
        size_=`L
        fullWidth=true
        onClick=handleClick
        onChange={e => onChange(e)}>
        <Text size=`M weight=`Bold> "Create new counter" </Text>
        <Input
          type_="text"
          value=name
          onSubmit={e => onSubmit(e)}
          onChange={e => onChange(e)}
        />
      </Button>
      {visible
         ? <Modal
             header={<H2> {React.string("New counter")} </H2>}
             footer={<div />}
             onBackdropClick=handleBackdropClick>
             <Input type_="text" value=name onSubmit />
           </Modal>
         : React.null}
    </>;
  };
};

module CounterList = {
  type state = Api.t;

  type actions =
    | SuccessData(Counter_t.counters)
    | ErrorData(string)
    | LoadingData;

  let updateState = (_state, action) => {
    switch (action) {
    | SuccessData(data) => RemoteData.Success(data)
    | ErrorData(err) => RemoteData.Failure(err)
    | LoadingData => RemoteData.Loading
    };
  };

  [@react.component]
  let make = () => {
    let (state, dispatch) = React.useReducer(updateState, NotFired);

    Hooks.useMount(() => {
      dispatch(LoadingData);
      Js.Promise.(
        Api.fetchCounters()
        |> then_(res => SuccessData(res) |> dispatch |> resolve)
        |> catch(err =>
             ErrorData(RemoteData.fromPromiseError(err))
             |> dispatch
             |> resolve
           )
      )
      ->ignore;
    });

    <Stack gap=2>
      <NewCounter />
      <CreateCounter />
      {switch (state) {
       | NotFired
       | Loading => <Text> "Loading" </Text>
       | Failure(e) => <Text> {"There has been an error: " ++ e} </Text>
       | Success(counters) =>
         counters.counters
         ->Belt.List.mapWithIndex((i, counter) =>
             <CounterItem
               key={i->string_of_int}
               name={counter.name}
               value={counter.value}
               id={counter.id}
             />
           )
         ->listToReactArray
       }}
    </Stack>;
  };
};

[@react.component]
let make = () => {
  <Align>
    <Layout>
      <H1> {React.string("Counters")} </H1>
      <Spacer bottom=4 />
      <CounterList />
    </Layout>
  </Align>;
};
