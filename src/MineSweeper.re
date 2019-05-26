open Revery;
open Revery.UI;
open Logic;

type endCondition =
  | No
  | Victory
  | Defeat

type state = {
  board: array(array(MineCell.cell)),
  width: int,
  height: int,
  ended: endCondition,
  time: Time.t,
};

type action =
  | Click(int, int)
  | Tick(Time.t)
  | ToogleFlag(int, int)

let component = React.component("Board");

let viewStyle =
  Style.[
    flexDirection(`Column), // it might be useless
  ];

let textStyle =
  Style.[
    color(Colors.white),
    fontFamily("Roboto-Regular.ttf"),
    fontSize(24),
  ];

let reducer(a, s) =
  switch a {
  | Click(j, i) => {
      if (s.ended !== No)
          s // no update
      else {
        let board = s.board;
        if (board[j][i].cellType === MineCell.Bomb)
          Logic.showCell(board, j, i)
        else
          Logic.propagateOpen(board, j, i, s.height - 1, s.width - 1);
        {
          ...s,
          board: board,
          ended: if (board[j][i].cellType === MineCell.Bomb) Defeat else No,
        }
      }
    }
  | Tick(t) => {
      if (s.ended !== No)
          s // Tweak while I learned how to cancel Hook.tick
      else
        {...s, time: Time.increment(s.time, t)}
    }
  | ToogleFlag(j, i) => {
      let board = s.board;
      if (!board[j][i].opened)
        Logic.toogleFlag(board, j, i);
      {
        ...s,
        board: board,
      }
  }
};

let createElement = (~children as _, ()) =>
  {
    let width = 10;
    let height = 10;
    let initialState = {
      board: minesweeper(width, height, 10),
      width,
      height,
      ended: No,
      time: Time.ofSeconds(0.),
    };
    component(hooks => {
      let (state, dispatch, hooks) = Hooks.reducer(~initialState,
         reducer, hooks);
      let hooks = Hooks.tick(~tickRate=Seconds(1.0), fun(t) => {
        if (state.ended === No)
          {
            // print_endline("I shouln't be fired when state.ended == true");
            dispatch(Tick(t));
          }
        }, hooks);
      (hooks,
        {
          let toRow(j, row) = {
            let row = Array.to_list(Array.mapi((i, e) =>
              <MineCell state=e
                onClick={
                  if (state.ended === No)
                    (fun() => dispatch(Click(j, i)))
                  else
                    ignore
                  }
                onOtherClick={
                  if (state.ended === No)
                    (fun() => dispatch(ToogleFlag(j, i)))
                  else
                    ignore
                  }/>, row));
            <MineRow> ...row </MineRow>
          };
          let rows = Array.to_list(Array.mapi(toRow, state.board));
          let rows = [<Text text={Printf.sprintf("%.0f s", Time.toSeconds(state.time))} style=textStyle />,
            ...rows];
          <View style=viewStyle>
            //<Text text="test." style=textStyle onMouseDown={fun(_) => dispatch(())}/>
            ...rows
          </View>
        });
    });
  }
