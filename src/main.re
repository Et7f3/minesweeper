open Revery;
open Revery.UI;

module Main = {
  type state = {
    frame: int
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

  let reducer = fun(a:_, s) => {frame: s.frame + 1};

  let createElement = (~children as _, ()) =>
    component(hooks => {
      let (state, dispatch, hooks) = Hooks.reducer(~initialState=({frame: 0}),
         reducer, hooks);
      (hooks,
        <View style=viewStyle>
          //<Text text="test." style=textStyle onMouseDown={fun(_) => dispatch(())}/>
          <MineRow> <MineCell /> <MineCell /> <MineCell /> <MineCell /> <MineCell /> </MineRow>
          <MineRow> <MineCell /> <MineCell /> <MineCell /> <MineCell /> <MineCell /> </MineRow>
          <MineRow> <MineCell /> <MineCell /> <MineCell /> <MineCell /> <MineCell /> </MineRow>
        </View>);
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
