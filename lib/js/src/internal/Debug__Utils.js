'use strict';

var $$String   = require("bs-platform/lib/js/string.js");
var Caml_array = require("bs-platform/lib/js/caml_array.js");
var Caml_int32 = require("bs-platform/lib/js/caml_int32.js");

var _m = 1000 * 60;

var _h = _m * 60;

var _d = _h * 24;

function _format(t, d, unit) {
  var s = String(Math.round(t / d));
  return "" + (String(s) + (" " + (String(unit) + "")));
}

function ms(ms$1) {
  if (ms$1 >= _d) {
    return _format(ms$1, _d, "d");
  } else if (ms$1 >= _h || ms$1 >= _m) {
    return _format(ms$1, _h, "h");
  } else if (ms$1 >= 1000) {
    return _format(ms$1, 1000, "s");
  } else {
    return _format(ms$1, 1, "ms");
  }
}

var Format = /* module */[
  /* _s */1000,
  /* _m */_m,
  /* _h */_h,
  /* _d */_d,
  /* _format */_format,
  /* ms */ms
];

function hash(input) {
  var value = [0];
  $$String.iter((function (c) {
          value[0] = ((value[0] << 5) - value[0] | 0) + c | 0;
          return /* () */0;
        }), input);
  return Math.abs(value[0]);
}

function selectColor(colors, string) {
  var choices = colors.length;
  var index = Caml_int32.mod_(hash(string), choices);
  return Caml_array.caml_array_get(colors, index);
}

function timer() {
  var prev = [Date.now()];
  return (function () {
      var now = Date.now();
      var delta = now - prev[0];
      prev[0] = now;
      return delta;
    });
}

exports.Format      = Format;
exports.hash        = hash;
exports.selectColor = selectColor;
exports.timer       = timer;
/* No side effect */
