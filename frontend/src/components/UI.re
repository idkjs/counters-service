let toPx = v => v ++ "px";
let stringToPx = v => v |> string_of_int |> toPx;
let unit = 8;
let unitize = v => v * unit;
let unitizeWithDefault = (v, def) =>
  switch (v) {
  | Some(v) => unitize(v)
  | None => def
  };

module Spacing = {
  let increment = 1.5;
  let s: float = unit |> float_of_int;
  let xs: float = s /. 1.5;
  let m: float = s *. increment;
  let l: float = m *. increment;
  let xl: float = l *. increment;
  let xxl: float = xl *. increment;
};

module Align = [%styled.div
  (~distribute=`Center, ~align=`Center) => [|
    [%css "display: flex"],
    [%css "height: 100%"],
    [%css "width: 100%;"],
    switch (distribute) {
    | `Start =>
      %css
      "justify-content: flex-start"
    | `Center =>
      %css
      "justify-content: center"
    | `End =>
      %css
      "justify-content: flex-end"
    },
    switch (align) {
    | `Start =>
      %css
      "align-items: flex-start"
    | `Center =>
      %css
      "align-items: center"
    | `End =>
      %css
      "align-items: flex-end"
    },
  |]
];

module T = [%styled.span
  (~size_=`M, ~align=`Left, ~weight=`Normal, ~color) => [|
    CssJs.unsafe("color", color),
    [%css "line-height: 1.5"],
    switch (size_) {
    | `S =>
      %css
      "font-size: 14px"
    | `M =>
      %css
      "font-size: 18px"
    | `L =>
      %css
      "font-size: 25px"
    },
    switch (align) {
    | `Left =>
      %css
      "text-align: left"
    | `Right =>
      %css
      "text-align: right"
    | `Center =>
      %css
      "text-align: center"
    },
    switch (weight) {
    | `Normal =>
      %css
      "font-weight: 400"
    | `Bold =>
      %css
      "font-weight: 700"
    },
  |]
];

module Text = {
  [@react.component]
  let make =
      (
        ~children,
        ~size as size_=`M,
        ~align=`Left,
        ~weight=`Normal,
        ~color="black",
      ) => {
    <T size_ align weight color> {React.string(children)} </T>;
  };
};

module Spacer = [%styled.div
  (~top=0, ~left=0, ~bottom=0, ~right=0, ~all=0) => [|
    switch (
      unitize(top),
      unitize(left),
      unitize(right),
      unitize(bottom),
      unitize(all),
    ) {
    | (v, _, _, _, _) when v != 0 =>
      CssJs.unsafe("marginTop", v |> stringToPx)
    | (_, v, _, _, _) when v != 0 =>
      CssJs.unsafe("marginLeft", v |> stringToPx)
    | (_, _, v, _, _) when v != 0 =>
      CssJs.unsafe("marginRight", v |> stringToPx)
    | (_, _, _, v, _) when v != 0 =>
      CssJs.unsafe("marginBottom", v |> stringToPx)
    | (_, _, _, _, v) when v != 0 => CssJs.unsafe("margin", v |> stringToPx)
    },
  |]
];

module S = [%styled.div
  (~fullHeight=true, ~align=`Top, ~distribute=`Center, ~gap) => [|
    [%css "display: flex"],
    [%css "flex-direction: column"],
    CssJs.unsafe("gap", gap),
    fullHeight ? [%css "height: 100%"] : [%css "height: auto"],
    switch (align) {
    | `Top =>
      %css
      "align-items: flex-start"
    | `Center =>
      %css
      "align-items: center"
    | `Bottom =>
      %css
      "align-items: flex-end"
    },
    switch (distribute) {
    | `Around =>
      %css
      "justify-content: space-around"
    | `Between =>
      %css
      "justify-content: space-between"
    | `Evenly =>
      %css
      "justify-content: space-evenly"
    | `Start =>
      %css
      "justify-content: flex-start"
    | `Center =>
      %css
      "justify-content: center"
    | `End =>
      %css
      "justify-content: flex-end"
    },
  |]
];

module Stack = {
  [@react.component]
  let make =
      (~fullHeight=true, ~align=`Top, ~distribute=`Center, ~gap=0, ~children) => {
    let bottom = unitize(gap) |> stringToPx;
    <S fullHeight align distribute gap=bottom> children </S>;
  };
};

module R = [%styled.div
  (~fullWidth=true, ~align=`Left, ~distribute=`Top, ~gap) => [|
    [%css "display: flex"],
    [%css "flex-direction: row"],
    CssJs.unsafe("gap", gap),
    fullWidth ? [%css "width: 100%"] : [%css "width: auto"],
    switch (align) {
    | `Left =>
      %css
      "align-items: flex-start"
    | `Center =>
      %css
      "align-items: center"
    | `Right =>
      %css
      "align-items: flex-end"
    },
    switch (distribute) {
    | `Around =>
      %css
      "justify-content: space-around"
    | `Between =>
      %css
      "justify-content: space-between"
    | `Evenly =>
      %css
      "justify-content: space-evenly"
    | `Top =>
      %css
      "justify-content: flex-start"
    | `Center =>
      %css
      "justify-content: center"
    | `Bottom =>
      %css
      "justify-content: flex-end"
    },
  |]
];

module Row = {
  [@react.component]
  let make =
      (~fullWidth=true, ~align=`Center, ~distribute=`Top, ~gap=0, ~children) => {
    let bottom = unitize(gap) |> stringToPx;
    <R fullWidth align distribute gap=bottom> children </R>;
  };
};
