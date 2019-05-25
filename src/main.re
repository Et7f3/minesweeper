open Revery;
open Revery.UI;

module Main = {
  let viewStyle =
    Style.[
      position(`Absolute),
      backgroundColor(Color.rgb(0., 0., 0.)),
      bottom(0),
      top(0),
      left(0),
      right(0),
      //flexDirection(`Column), // or `Row
    ];
  let createElement(~children, ()) = <View style=viewStyle> <Board /> </View>
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
