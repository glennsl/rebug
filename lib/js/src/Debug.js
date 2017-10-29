'use strict';

var List           = require("bs-platform/lib/js/list.js");
var Curry          = require("bs-platform/lib/js/curry.js");
var Debug__Utils   = require("./internal/Debug__Utils.js");
var Debug__Browser = require("./internal/Debug__Browser.js");

var instances = [/* [] */0];

var names = [/* [] */0];

var skips = [/* [] */0];

function _isForceEnabled(namespace) {
  return +(namespace[namespace.length] === "*");
}

function _isSkipped(namespace) {
  return List.exists((function (param) {
                return +param.test(namespace);
              }), skips[0]);
}

function isEnabled(namespace) {
  if (_isForceEnabled(namespace)) {
    return /* true */1;
  } else if (_isSkipped(namespace)) {
    return /* false */0;
  } else {
    var namespace$1 = namespace;
    return List.exists((function (param) {
                  return +param.test(namespace$1);
                }), names[0]);
  }
}

function make(namespace) {
  var timer = Debug__Utils.timer(/* () */0);
  var instance = /* record */[
    /* namespace */namespace,
    /* color */Debug__Utils.selectColor(Debug__Browser.colors, namespace),
    /* enabled */isEnabled(namespace)
  ];
  instances[0] = /* :: */[
    instance,
    instances[0]
  ];
  var log = function (message, args) {
    if (instance[/* enabled */2]) {
      var delta = Debug__Utils.Format[/* ms */5](Curry._1(timer, /* () */0));
      var formatted = Debug__Browser.format(namespace, delta, message);
      var color = "color:" + instance[/* color */1];
      return Debug__Browser.log(/* :: */[
                  formatted,
                  /* :: */[
                    color,
                    /* :: */[
                      "color: inherit",
                      /* :: */[
                        color,
                        args
                      ]
                    ]
                  ]
                ]);
    } else {
      return 0;
    }
  };
  return /* record */[
          /* log */(function (m) {
              return log(m, /* [] */0);
            }),
          /* log2 */(function (m, a) {
              return log(m, /* :: */[
                          a,
                          /* [] */0
                        ]);
            }),
          /* log3 */(function (m, a, b) {
              return log(m, /* :: */[
                          a,
                          /* :: */[
                            b,
                            /* [] */0
                          ]
                        ]);
            }),
          /* log4 */(function (m, a, b, c) {
              return log(m, /* :: */[
                          a,
                          /* :: */[
                            b,
                            /* :: */[
                              c,
                              /* [] */0
                            ]
                          ]
                        ]);
            }),
          /* logMany */log,
          /* isEnabled */(function () {
              return instance[/* enabled */2];
            }),
          /* enable */(function () {
              instance[/* enabled */2] = /* true */1;
              return /* () */0;
            })
        ];
}

function _resetNamespaces() {
  names[0] = /* [] */0;
  skips[0] = /* [] */0;
  return /* () */0;
}

function _parseFilter(filter) {
  filter.split((/[/s,]+/)).filter((function (ns) {
            return +(ns !== "");
          })).forEach((function (namespace) {
          var namespace$1 = namespace.replace((/\*/g), ".*?");
          if (namespace$1[0] === "-") {
            var namespace$2 = namespace$1.substr(1);
            skips[0] = /* :: */[
              new RegExp("^" + (String(namespace$2) + "\$")),
              skips[0]
            ];
            return /* () */0;
          } else {
            names[0] = /* :: */[
              new RegExp("^" + (String(namespace$1) + "\$")),
              names[0]
            ];
            return /* () */0;
          }
        }));
  return /* () */0;
}

function enable(filter) {
  Debug__Browser.save(/* Some */[filter]);
  _resetNamespaces(/* () */0);
  _parseFilter(filter);
  return List.iter((function (i) {
                i[/* enabled */2] = isEnabled(i[/* namespace */0]);
                return /* () */0;
              }), instances[0]);
}

function disable() {
  Debug__Browser.save(/* None */0);
  _resetNamespaces(/* () */0);
  return List.iter((function (i) {
                i[/* enabled */2] = /* false */0;
                return /* () */0;
              }), instances[0]);
}

var match = Debug__Browser.load(/* () */0);

if (match) {
  enable(match[0]);
}

exports.make      = make;
exports.enable    = enable;
exports.disable   = disable;
exports.isEnabled = isEnabled;
/* match Not a pure module */
