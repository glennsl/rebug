'use strict';

var Curry      = require("bs-platform/lib/js/curry.js");
var Debug      = require("../src/Debug.js");
var Caml_int32 = require("bs-platform/lib/js/caml_int32.js");

var a = Debug.make("debug:a");

var b = Debug.make("debug:b");

var obj = {
  foo: 42,
  bar: /* true */1
};

var record = /* record */[
  /* foo */24,
  /* bar : false */0
];

function ppRecord(param) {
  return "{ foo: " + (String(param[/* foo */0]) + (", bar: " + (String(param[/* bar */1]) + " }")));
}

Curry._1(a[/* log1 */0], "The red cow jumps over the lazy giraffe");

Curry._2(b[/* log2 */1], "wat", obj);

var square = Curry._4(a[/* fn1 */15], "square", /* None */0, /* None */0, (function (x) {
        return Caml_int32.imul(x, x);
      }));

var powSquared = Curry._5(a[/* fn2 */16], "powSquared", /* None */0, /* None */0, /* None */0, (function (x, y) {
        return Math.pow(Curry._1(square, x), Curry._1(square, y));
      }));

console.log(Curry._2(powSquared, 4, 3));

Curry._1(a[/* lazy1 */10], (function () {
        return "lazy";
      }));

Curry._1(a[/* lazy2 */11], (function () {
        return /* tuple */[
                "lazy",
                obj
              ];
      }));

Curry._2(b[/* error2 */6], "boo!", obj);

var doThings = Curry._5(b[/* fn2 */16], "doThings", /* Some */[ppRecord], /* None */0, /* None */0, (function (r, n) {
        Curry._2(b[/* log2 */1], "foo: ", r[/* foo */0]);
        return Curry._2(powSquared, r[/* foo */0], n);
      }));

Curry._2(doThings, record, 2);

var loggedParse = Curry._4(a[/* fn1 */15], "JSON.parse", /* None */0, /* None */0, (function (prim) {
        return JSON.parse(prim);
      }));

console.log(Curry._1(loggedParse, "{ \"foo\": \"bar\" }"));

exports.a           = a;
exports.b           = b;
exports.obj         = obj;
exports.record      = record;
exports.ppRecord    = ppRecord;
exports.square      = square;
exports.powSquared  = powSquared;
exports.doThings    = doThings;
exports.loggedParse = loggedParse;
/* a Not a pure module */
