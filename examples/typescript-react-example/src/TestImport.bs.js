// Generated by BUCKLESCRIPT VERSION 5.0.0, PLEASE EDIT WITH CARE

import * as Caml_option from "bs-platform/lib/es6/caml_option.js";
import * as TestImportGen from "./TestImport.gen";
import * as ImportMyBannerGen from "./ImportMyBanner.gen";

var innerStuffContents = TestImportGen.innerStuffContents;

function make(prim, prim$1, prim$2) {
  return ImportMyBannerGen.make(prim, prim$1 !== undefined ? Caml_option.valFromOption(prim$1) : undefined, prim$2);
}

export {
  innerStuffContents ,
  make ,
  
}
/* innerStuffContents Not a pure module */
