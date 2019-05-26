open Revery;
open Revery.UI;
open Logic;

type endCondition =
  | No
  | Victory
  | Defeat

let endConditionToColor = fun
  | No => Colors.white
  | Victory => Colors.green
  | Defeat => Colors.red;


let endConditionToString = fun
  | No => ""
  | Victory => "Well played"
  | Defeat => "You have loosed";

type state = {
  board: array(array(MineCell.cell)),
  width: int,
  height: int,
  ended: endCondition,
  time: Time.t,
  numberOfFlag: int,
  numberOfBomb: int,
  remainingCell: int,
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
        let rmnCell = ref(s.remainingCell);
        if (board[j][i].cellType === MineCell.Bomb)
          Logic.showCell(board, j, i, rmnCell)
        else
          Logic.propagateOpen(board, j, i, s.height - 1, s.width - 1, rmnCell);
        {
          ...s,
          board: board,
          remainingCell: rmnCell^,
          ended:
            if (board[j][i].cellType === MineCell.Bomb)
              Defeat
            else if (rmnCell^ === s.numberOfBomb)
              Victory
            else
              No,
        }
      }
    }
  | Tick(t) => {
      if (s.ended !== No)
          s // Tweak while I learned how to cancel Hook.tick
      else
        {
          ...s,
          time: Time.increment(s.time, t),
        }
    }
  | ToogleFlag(j, i) => {
      let board = s.board;
      let nbrFlag = ref(s.numberOfFlag);
      if (!board[j][i].opened)
        Logic.toogleFlag(board, j, i, nbrFlag);
      {
        ...s,
        board: board,
        numberOfFlag: nbrFlag^,
      }
  }
};

let createElement = (~children as _, ()) =>
  {
    let width = 10;
    let height = 10;
    let (numberOfFlag, board) = minesweeper(width, height, 10);
    let initialState = {
      board,
      width,
      height,
      ended: No,
      time: Time.ofSeconds(0.),
      numberOfFlag,
      numberOfBomb: numberOfFlag,
      remainingCell: width * height,
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
              <MineCell state=e finish=(state.ended !== No)
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
          let rows =
            [
              <MineRow>
                <Text text={Printf.sprintf("%02d", state.numberOfFlag)} style=Style.[
                  width(100),
                  color(Colors.white),
                  ...textStyle,
                ] />
                <Text text={endConditionToString(state.ended)} style=Style.[
                  width(300),
                  color(endConditionToColor(state.ended)),
                  ...textStyle,
                ] />
                <Text text={Printf.sprintf("%03.0f s", Time.toSeconds(state.time))} style=Style.[
                  width(100),
                  ...textStyle,
                ] />
              </MineRow>,
              ...rows,
            ];
          <View style=viewStyle>
            ...rows
          </View>
        });
    });
  }
