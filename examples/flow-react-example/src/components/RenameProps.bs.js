// Generated by BUCKLESCRIPT VERSION 5.0.6, PLEASE EDIT WITH CARE

import * as ReasonReact from "reason-react/src/ReasonReact.js";

var component = ReasonReact.statelessComponent("RenameProps");

function functionWithGenTypeAs(firstNameArgumentCantBeRenamed, type_, number) {
  return firstNameArgumentCantBeRenamed + (type_ + String(number));
}

function make(firstNameArgumentCantBeRenamed, type_, number, _children) {
  return /* record */[
          /* debugName */component[/* debugName */0],
          /* reactClassInternal */component[/* reactClassInternal */1],
          /* handedOffState */component[/* handedOffState */2],
          /* willReceiveProps */component[/* willReceiveProps */3],
          /* didMount */component[/* didMount */4],
          /* didUpdate */component[/* didUpdate */5],
          /* willUnmount */component[/* willUnmount */6],
          /* willUpdate */component[/* willUpdate */7],
          /* shouldUpdate */component[/* shouldUpdate */8],
          /* render */(function (_self) {
              return firstNameArgumentCantBeRenamed + (type_ + String(number));
            }),
          /* initialState */component[/* initialState */10],
          /* retainedProps */component[/* retainedProps */11],
          /* reducer */component[/* reducer */12],
          /* jsElementWrapped */component[/* jsElementWrapped */13]
        ];
}

function firstIsIgnored(x) {
  return x + 0 | 0;
}

function padding1(pad, x) {
  return pad + x | 0;
}

function padding2(pad, x) {
  return pad + x | 0;
}

function padding3(pad1, pad2, x) {
  return (pad1 + pad2 | 0) + x | 0;
}

function renameABunch(pad, x, y, z) {
  return ((pad + x | 0) + y | 0) + z | 0;
}

function renameABunch2(pad, x, y, z) {
  return ((pad + x | 0) + y | 0) + z | 0;
}

function renameABunch3(pad, x, y, z) {
  return ((pad + x | 0) + y | 0) + z | 0;
}

export {
  component ,
  functionWithGenTypeAs ,
  make ,
  firstIsIgnored ,
  padding1 ,
  padding2 ,
  padding3 ,
  renameABunch ,
  renameABunch2 ,
  renameABunch3 ,
  
}
/* component Not a pure module */
