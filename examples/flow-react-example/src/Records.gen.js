/** 
 * @flow strict
 * @generated
 * @nolint
 */
/* eslint-disable */
// $FlowExpectedError: Reason checked type sufficiently
type $any = any;

// $FlowExpectedError: Reason checked type sufficiently
import * as Curry from 'bs-platform/lib/es6/curry.js';

// $FlowExpectedError: Reason checked type sufficiently
import * as RecordsBS from './Records.bs';

// flowlint-next-line nonstrict-import:off
import type {weekday as Types_weekday} from './Types.gen';

export type coord = {|
  +x: number, 
  +y: number, 
  +z?: number
|};

export type coord2 = {|
  +a: number, 
  +b: number, 
  +c: ?number
|};

export type bigType = {|
  +x: number, 
  +y: number, 
  +z: ?{|
    +x: number, 
    +y: number, 
    +z?: {|
      +x: number, 
      +y: number, 
      +z: ?number
    |}
  |}
|};

export type testMutable = {| mutableField: number, +immutableField: number |};

export opaque type innerRecord = mixed;

export type outerRecord = {| +innerRecord: innerRecord |};

export const origin: coord = {x:RecordsBS.origin[0], y:RecordsBS.origin[1], z:RecordsBS.origin[2]};

export const computeArea: (coord) => number = function (Arg1: $any) {
  const result = RecordsBS.computeArea([Arg1.x, Arg1.y, Arg1.z]);
  return result
};

export const coord2d: (number, number) => coord = function (Arg1: $any, Arg2: $any) {
  const result = Curry._2(RecordsBS.coord2d, Arg1, Arg2);
  return {x:result[0], y:result[1], z:result[2]}
};

export const computeArea2: (coord2) => number = function (Arg1: $any) {
  const result = RecordsBS.computeArea2([Arg1.a, Arg1.b, Arg1.c]);
  return result
};

export const computeArea3: ({|
  +x: number, 
  +y: number, 
  +z: ?number
|}) => number = RecordsBS.computeArea3;

export const computeArea4: ({|
  +x: number, 
  +y: number, 
  +z?: number
|}) => number = RecordsBS.computeArea4;

export const computeNested: ({|
  +x: number, 
  +y: number, 
  +z?: {|
    +x: number, 
    +y: number, 
    +z?: number
  |}
|}) => number = RecordsBS.computeNested;

export const computeNestedNested: ({|
  +x: number, 
  +y: number, 
  +z?: {|
    +x: number, 
    +y: number, 
    +z?: {|
      +x: number, 
      +y: number, 
      +z?: number
    |}
  |}
|}) => number = RecordsBS.computeNestedNested;

export const computeNestedNestedNullable: ({|
  +x: number, 
  +y: number, 
  +z: ?{|
    +x: number, 
    +y: number, 
    +z: ?{|
      +x: number, 
      +y: number, 
      +z: ?number
    |}
  |}
|}) => number = RecordsBS.computeNestedNestedNullable;

export const computeNestedNestedHalfNullable: (bigType) => number = RecordsBS.computeNestedNestedHalfNullable;

export const useTypeImportedInOtherModule: (Types_weekday) => Types_weekday = RecordsBS.useTypeImportedInOtherModule;

export const convertInner: (innerRecord) => innerRecord = RecordsBS.convertInner;

export const convertOuter: (outerRecord) => outerRecord = function (Arg1: $any) {
  const result = RecordsBS.convertOuter([Arg1.innerRecord]);
  return {innerRecord:result[0]}
};
