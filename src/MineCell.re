open Revery;
open Revery.UI;

let defaultViewStyle =
  Style.[
    width(50),
    height(50),
  ];

let openViewStyle =
  Style.[
    backgroundColor(Color.hex("#ffffff")),
    ...defaultViewStyle
  ];

let closedViewStyle =
  Style.[
    backgroundColor(Color.hex("#000")),
    ...defaultViewStyle
  ];

type cellType =
  | Bomb
  | Hint(int);

type cell = {
  opened: bool,
  cellType: cellType,
};


let component = React.component("MineCell");

let reducer = fun(a:_, s) => {...s, opened: true};

let createElement = (~children as _, ~y, ~x, ~state, ()) =>
  component(hooks => {
    let (state, dispatch, hooks) = Hooks.reducer(~initialState=state,
       reducer, hooks);
    (hooks,
      <View style={if (state.opened) openViewStyle else closedViewStyle}>
      </View>);
  });
