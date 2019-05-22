open Revery;
open Revery.UI;

let () = Random.self_init()

let count_neighbours(matrix, w, h, y, x) =
    (if (x > 0)
        ((if (y > 0 && matrix[y - 1][x - 1]) 1 else 0)
        + (if (matrix[y][x - 1]) 1 else 0)
        + (if (y < h && matrix[y + 1][x - 1]) 1 else 0))
    else
        0)
    + (if (y > 0 && matrix[y - 1][x]) 1 else 0)
    + (if (y < h && matrix[y + 1][x]) 1 else 0)
    + (if (x < w)
        ((if (y > 0 && matrix[y - 1][x + 1]) 1 else 0)
        + (if (matrix[y][x + 1]) 1 else 0)
        + (if (y < h && matrix[y + 1][x + 1]) 1 else 0))
    else
        0);

let minesweeper = (w, h, percent) => {
  let percent = 101 - percent;
  let matrix = Array.make_matrix(w, h, false);
  let (w, h) = (w - 1, h - 1);
  let matrix = Array.map(Array.map(_ => Random.int(100) >= percent), matrix); /* fill bomb */
  let count_neighbours = count_neighbours(matrix, h, w);
  let make_cell = (i, j) =>
    fun
    | true => MineCell.Bomb
    | false => MineCell.Hint(count_neighbours(i, j));
  let make_cell(i, j, v) = MineCell.{opened: false, cellType: make_cell(i, j, v)};
  let make_row = i => Array.mapi(make_cell(i));
  let matrix = Array.mapi(make_row, matrix);
  matrix;
};

module Main = {
  type state = {
    board: array(array(MineCell.cell)),
  };

  let component = React.component("Main");

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

  let reducer = fun(a:_, s) => {
    let board = s.board;
    board[0][0] = {
      ...(board[0][0]), opened : true
    };
    {board: board}
  };

  let createElement = (~children as _, ()) =>
    component(hooks => {
      let (state, dispatch, hooks) = Hooks.reducer(~initialState=({board: minesweeper(10, 10, 30)}),
         reducer, hooks);
      (hooks,
        {
          let row = Array.to_list(Array.mapi((i, _) => <MineCell x=i/>, state.board[0]));
          <View style=viewStyle>
            //<Text text="test." style=textStyle onMouseDown={fun(_) => dispatch(())}/>
            <MineRow> ...row </MineRow>
            <MineRow> <MineCell x=(-1) /> <MineCell x=(-1) /> <MineCell x=(-1) /> <MineCell x=(-1) /> <MineCell x=(-1) /> </MineRow>
            <MineRow> <MineCell x=(-1) /> <MineCell x=(-1) /> <MineCell x=(-1) /> <MineCell x=(-1) /> <MineCell x=(-1) /> </MineRow>
          </View>
        });
    });
  };

let init = app => {
  let dimensions: Monitor.size =
    Monitor.getPrimaryMonitor() |> Monitor.getSize;

  let windowWidth = dimensions.width / 2;
  let windowHeight = dimensions.height / 2;

  let win =
    App.createWindow(
      ~createOptions =
        WindowCreateOptions.create(
          ~width = windowWidth,
          ~height = windowHeight,
          (),
        ),
      app,
      "Welcome to Minesweeper!",
    );

  let _ = UI.start(win, <Main />);
  ();
};

App.start(init);
