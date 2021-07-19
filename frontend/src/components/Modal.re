module Backdrop = [%styled.div
  {|
  background-color: rgba(34, 42, 40, 0.7);
  position: fixed;
  left: 0;
  top: 0;
  width: 100%;
  height: 100%;
  display: flex;
  align-items: flex-start;
  justify-content: center;
  z-index: 9;
|}
];

module Float = [%styled.div
  {|
  position: fixed;
  left: 0px;
  top: 0px;
  z-index: 10;
|}
];

module Wrapper = [%styled.div
  {|
  border-radius: 6px;
  max-width: 28rem;
  max-height: 90vh;
  margin-top: 5vh;
  overflow: auto;
  width: 100%;
  flex-shrink: 0;
  background-color: white;
|}
];

module Header = [%styled.div
  {|
  position: sticky;
  top: 0px;
  background-color: white;
  padding: 28px 28px;
  padding-bottom: 0px;
  z-index: 11;
|}
];

module Content = [%styled.div {|
  padding: 28px 28px;
|}];

module Footer = [%styled.div
  {|
  padding: 28px 28px;
  padding-top: 0px;
  position: sticky;
  top: 0px;
  background-color: white;
  padding-bottom: 0px;
  z-index: 1;
|}
];

[@react.component]
let make = (~header, ~footer, ~children, ~onBackdropClick) => {
  <Float>
    <Backdrop onClick=onBackdropClick>
      <Wrapper>
        <Header> header </Header>
        <Content> children </Content>
        <Footer> footer </Footer>
      </Wrapper>
    </Backdrop>
  </Float>;
};
