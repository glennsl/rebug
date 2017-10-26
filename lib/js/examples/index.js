'use strict';

var Curry = require("bs-platform/lib/js/curry.js");
var Debug = require("../src/Debug.js");

var a = Debug.make("debug:a");

var b = Debug.make("debug:b");

Curry._1(a[/* log */0], "The red cow jumps over the lazy giraffe");

Curry._2(b[/* log2 */1], "wat", {
      foo: 42,
      bar: /* true */1
    });

exports.a = a;
exports.b = b;
/* a Not a pure module */
