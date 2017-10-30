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

Curry._1(a[/* log1 */0], "The red cow jumps over the lazy giraffe");

Curry._2(b[/* log2 */1], "wat", obj);

var square = Curry._2(a[/* fn1 */15], "square", (function (x) {
        return Caml_int32.imul(x, x);
      }));

var powSquared = Curry._2(a[/* fn2 */16], "powSquared", (function (x, y) {
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

exports.a          = a;
exports.b          = b;
exports.obj        = obj;
exports.square     = square;
exports.powSquared = powSquared;
/* a Not a pure module */
