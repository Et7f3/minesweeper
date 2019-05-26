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

let textStyle =
  Style.[
    color(Colors.red),
    fontFamily("Roboto-Regular.ttf"),
    fontSize(24),
  ];

type cellType =
  | Bomb
  | Flag
  | Hint(int);

type cell = {
  opened: bool,
  cellType: cellType,
};

let createElement = (~children as _, ~state, ~onClick, ()) =>
  {
    let children = switch(state)
    {
      | {cellType: Flag} => [<Text style=textStyle text="?" />]
      | {opened: false} | {cellType: Hint(0), opened: true} => [] // nothing to display
      | {cellType: Hint(n), opened: true} => [<Text style=textStyle text={string_of_int(n)} />]
      | {cellType: Bomb, opened: true} => [<Text style=textStyle text="O" />]
    };
    <View style={if (state.opened) openViewStyle else closedViewStyle} onMouseDown={fun(_) => onClick()}>
    ...children</View>
  };
