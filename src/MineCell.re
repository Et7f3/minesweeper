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

let createElement = (~children as _, ~state, ~onClick, ()) =>
  <View style={if (state.opened) openViewStyle else closedViewStyle} onMouseDown={fun(_) => onClick()}>
  </View>;
