'use strict';

var Curry = require("bs-platform/lib/js/curry.js");
var Debug = require("../src/Debug.js");

var debug = Debug.make("my:namespace");

Curry._2(debug[/* log2 */1], "wat", {
      foo: 42
    });

Curry._1(debug[/* log */0], "bar");

Curry._2(debug[/* logMany */4], "msg", /* :: */[
      1,
      /* :: */[
        "foo",
        /* :: */[
          {
            bar: 98
          },
          /* [] */0
        ]
      ]
    ]);

exports.debug = debug;
/* debug Not a pure module */
