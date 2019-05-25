open Revery;
open Revery.UI;

let textStyle =
  Style.[
    color(Colors.red),
    fontFamily("Roboto-Regular.ttf"),
    fontSize(24),
  ];

type state = {
  time: Time.t
}

type action = Update(Time.t);

let reducer(a, s) =
  switch(a) {
  | Update(t) => {time: Time.increment(s.time, t)}
};

let initialState = {time: Time.ofSeconds(0.)};

let component = React.component("Timer");

let createElement = (~children as _, ()) => component(hooks => {
  let (state, dispatch, hooks) = Hooks.reducer(~initialState, reducer, hooks);
  let hooks = Hooks.tick(~tickRate=Seconds(1.0), fun(t) => dispatch(Update(t)), hooks);
  (hooks,
    <View> <Text text={Printf.sprintf("%.0f s", Time.toSeconds(state.time))} style=textStyle /> </View>,
  )
});
