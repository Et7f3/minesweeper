open Revery;
open Revery.UI;

let viewStyle =
  Style.[
    width(50),
    height(50),
    backgroundColor(Color.hex("#ffffff")),
  ];

type cellType =
  | Bomb
  | Hint(int);

type cell = {
  opened: bool,
  cellType: cellType,
};

type state = {
  frame: int
}

let component = React.component("MineCell");

let reducer = fun(a:_, s) => {frame: s.frame + 1};

let createElement = (~children as _, ~y, ~x, ()) =>
  component(hooks => {
    let (state, dispatch, hooks) = Hooks.reducer(~initialState=({frame: 0}),
       reducer, hooks);
    (hooks,
      <View style=viewStyle>
      </View>);
  });
