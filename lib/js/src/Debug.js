'use strict';

var List           = require("bs-platform/lib/js/list.js");
var Curry          = require("bs-platform/lib/js/curry.js");
var Debug__Utils   = require("./internal/Debug__Utils.js");
var Debug__Browser = require("./internal/Debug__Browser.js");

function Env_003(prim) {
  console.group(prim);
  return /* () */0;
}

function Env_004() {
  console.groupEnd();
  return /* () */0;
}

var instances = [/* [] */0];

var names = [/* [] */0];

var skips = [/* [] */0];

function isForceEnabled(namespace) {
  return +(namespace[namespace.length] === "*");
}

function isSkipped(namespace) {
  return List.exists((function (param) {
                return +param.test(namespace);
              }), skips[0]);
}

function isEnabled(namespace) {
  if (isForceEnabled(namespace)) {
    return /* true */1;
  } else if (isSkipped(namespace)) {
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
  var log = function (log$1, message, args) {
    var delta = Debug__Utils.Format[/* ms */5](Curry._1(timer, /* () */0));
    var formatted = Debug__Browser.format(namespace, delta, message);
    var color = "color:" + instance[/* color */1];
    return Curry._1(log$1, /* :: */[
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
  };
  var logIfEnabled = function (message, args) {
    var match = instance[/* enabled */2];
    if (match !== 0) {
      return log(Debug__Browser.log, message, args);
    } else {
      return /* () */0;
    }
  };
  var error = function (message, args) {
    return log(Debug__Browser.error, message, args);
  };
  var fnEnter = function (message, args) {
    Curry._1(Env_003, message);
    return logIfEnabled("->", args);
  };
  var fnExit = function (ret) {
    logIfEnabled("<-", /* :: */[
          ret,
          /* [] */0
        ]);
    Curry._1(Env_004, /* () */0);
    return ret;
  };
  return /* record */[
          /* log1 */(function (m) {
              return logIfEnabled(m, /* [] */0);
            }),
          /* log2 */(function (m, a) {
              return logIfEnabled(m, /* :: */[
                          a,
                          /* [] */0
                        ]);
            }),
          /* log3 */(function (m, a, b) {
              return logIfEnabled(m, /* :: */[
                          a,
                          /* :: */[
                            b,
                            /* [] */0
                          ]
                        ]);
            }),
          /* log4 */(function (m, a, b, c) {
              return logIfEnabled(m, /* :: */[
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
          /* logMany */logIfEnabled,
          /* error1 */(function (m) {
              return error(m, /* [] */0);
            }),
          /* error2 */(function (m, a) {
              return error(m, /* :: */[
                          a,
                          /* [] */0
                        ]);
            }),
          /* error3 */(function (m, a, b) {
              return error(m, /* :: */[
                          a,
                          /* :: */[
                            b,
                            /* [] */0
                          ]
                        ]);
            }),
          /* error4 */(function (m, a, b, c) {
              return error(m, /* :: */[
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
          /* errorMany */error,
          /* lazy1 */(function (f) {
              var match = instance[/* enabled */2];
              if (match !== 0) {
                return log(Debug__Browser.log, Curry._1(f, /* () */0), /* [] */0);
              } else {
                return /* () */0;
              }
            }),
          /* lazy2 */(function (f) {
              if (instance[/* enabled */2]) {
                var match = Curry._1(f, /* () */0);
                return log(Debug__Browser.log, match[0], /* :: */[
                            match[1],
                            /* [] */0
                          ]);
              } else {
                return 0;
              }
            }),
          /* lazy3 */(function (f) {
              if (instance[/* enabled */2]) {
                var match = Curry._1(f, /* () */0);
                return log(Debug__Browser.log, match[0], /* :: */[
                            match[1],
                            /* :: */[
                              match[2],
                              /* [] */0
                            ]
                          ]);
              } else {
                return 0;
              }
            }),
          /* lazy4 */(function (f) {
              if (instance[/* enabled */2]) {
                var match = Curry._1(f, /* () */0);
                return log(Debug__Browser.log, match[0], /* :: */[
                            match[1],
                            /* :: */[
                              match[2],
                              /* :: */[
                                match[3],
                                /* [] */0
                              ]
                            ]
                          ]);
              } else {
                return 0;
              }
            }),
          /* lazyMany */(function (f) {
              if (instance[/* enabled */2]) {
                var match = Curry._1(f, /* () */0);
                return log(Debug__Browser.log, match[0], match[1]);
              } else {
                return 0;
              }
            }),
          /* fn1 */(function (m, f, a) {
              fnEnter(m, /* :: */[
                    a,
                    /* [] */0
                  ]);
              return fnExit(Curry._1(f, a));
            }),
          /* fn2 */(function (m, f, a, b) {
              fnEnter(m, /* :: */[
                    a,
                    /* :: */[
                      b,
                      /* [] */0
                    ]
                  ]);
              return fnExit(Curry._2(f, a, b));
            }),
          /* fn3 */(function (m, f, a, b, c) {
              fnEnter(m, /* :: */[
                    a,
                    /* :: */[
                      b,
                      /* :: */[
                        c,
                        /* [] */0
                      ]
                    ]
                  ]);
              return fnExit(Curry._3(f, a, b, c));
            }),
          /* fn4 */(function (m, f, a, b, c, d) {
              fnEnter(m, /* :: */[
                    a,
                    /* :: */[
                      b,
                      /* :: */[
                        c,
                        /* :: */[
                          d,
                          /* [] */0
                        ]
                      ]
                    ]
                  ]);
              return fnExit(Curry._4(f, a, b, c, d));
            }),
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
