open Revery;
open Revery.UI;
open Logic;

type state = {
  board: array(array(MineCell.cell)),
  width: int,
  height: int,
  ended: bool
};

let component = React.component("Board");

let viewStyle =
  Style.[
    position(`Absolute),
    backgroundColor(Color.rgb(0., 0., 0.)),
    bottom(0),
    top(0),
    left(0),
    right(0),
    flexDirection(`Column),
  ];

let textStyle =
  Style.[
    color(Colors.white),
    fontFamily("Roboto-Regular.ttf"),
    fontSize(24),
  ];

let reducer = fun((j, i), s) => {
  let board = s.board;
  let () = flush(stdout);
  if (!board[j][i].opened)
    Logic.propagate_open(board, j, i, s.height - 1, s.width - 1);
  {...s, board: board}
};

let createElement = (~children as _, ()) =>
  {
    let width = 10;
    let height = 10;
    let initialState = {
      board: minesweeper(width, height, 10),
      width,
      height,
      ended: false
    };
    component(hooks => {
      let (state, dispatch, hooks) = Hooks.reducer(~initialState,
         reducer, hooks);
      (hooks,
        {
          let to_row(j, row) = {
            let row = Array.to_list(Array.mapi((i, e) => <MineCell state=e onClick={fun() => dispatch((j, i))}/>, row));
            <MineRow> ...row </MineRow>
          };
          let rows = Array.to_list(Array.mapi(to_row, state.board));
          <View style=viewStyle>
            //<Text text="test." style=textStyle onMouseDown={fun(_) => dispatch(())}/>
            ...rows
          </View>
        });
    });
  }
