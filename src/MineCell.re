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
    ...defaultViewStyle,
  ];

let closedViewStyle =
  {
    let w = 10;
    Style.[
      backgroundColor(Color.hex("#666666")),
      borderTop(~color=Color.hex("#767676"), ~width=w),
      borderLeft(~color=Color.hex("#767676"), ~width=w),
      borderBottom(~color=Color.hex("#565656"), ~width=w),
      borderRight(~color=Color.hex("#565656"), ~width=w),
      ...defaultViewStyle,
    ]
  };

let textStyle =
  Style.[
    color(Colors.red),
    fontFamily("Roboto-Regular.ttf"),
    fontSize(24),
  ];

type cellType =
  | Bomb
  | Flag(cellType)
  | Hint(int);

type cell = {
  opened: bool,
  cellType: cellType,
};

let createElement = (~children as _, ~state, ~finish, ~onClick, ~onOtherClick, ()) =>
  {
    let children = switch(state)
    {
      | {cellType: Bomb, opened} =>
        {
          if (opened || finish)
            [<Text style=textStyle text="O" />]
          else
            []
        }
      | {cellType: Flag(_), _} => [<Text style=textStyle text="?" />]
      | {opened: false, _} | {cellType: Hint(0), opened: true} => [] // nothing to display
      | {cellType: Hint(n), opened: true} => [<Text style=textStyle text={string_of_int(n)} />]
    };
    <View style={if (state.opened) openViewStyle else closedViewStyle} onMouseDown={fun(evt) => if (evt.button == BUTTON_LEFT) onClick() else onOtherClick()}>
    ...children</View>
  };
