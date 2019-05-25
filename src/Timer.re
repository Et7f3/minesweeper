open Revery;
open Revery.UI;

let textStyle =
  Style.[
    color(Colors.red),
    fontFamily("Roboto-Regular.ttf"),
    fontSize(24),
  ];

//let component = React.component("Timer"); // may be needed for hook

let createElement = (~children as _, ()) => <View> <Text text="timer" style=textStyle /> </View>;
