open Revery.UI;

let viewStyle =
  Style.[
    flexDirection(`Row),
  ];

let createElement = (~children, ()) => <View style=viewStyle> ...children </View>
