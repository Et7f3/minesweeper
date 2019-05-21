open Revery;
open Revery.UI;

module Main = {
  let component = React.component("Main");

    let createElement = (~children as _, ~win:_, ()) =>
      component(hooks => {
        ignore(win);
        //let (state, dispatch, hooks) = Hooks.reducer(~initialState = state, reducer, hooks);
        (hooks, <View />);
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

  let _ = UI.start(win, <Main win />);
  ();
};

App.start(init);
