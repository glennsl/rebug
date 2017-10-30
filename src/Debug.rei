type arg;

external arg : 'a => arg = "%identity";

type logger = {
  log1: string => unit,
  log2: 'a. (string, 'a) => unit,
  log3: 'a 'b. (string, 'a, 'b) => unit,
  log4: 'a 'b 'c. (string, 'a, 'b, 'c) => unit,
  logMany: (string, list(arg)) => unit,

  error1: string => unit,
  error2: 'a .(string, 'a) => unit,
  error3: 'a 'b .(string, 'a, 'b) => unit,
  error4: 'a 'b 'c .(string, 'a, 'b, 'c) => unit,
  errorMany: (string, list(arg)) => unit,

  lazy1: (unit => string) => unit,
  lazy2: 'a. (unit => (string, 'a)) => unit,
  lazy3: 'a 'b. (unit => (string, 'a, 'b)) => unit,
  lazy4: 'a 'b 'c. (unit => (string, 'a, 'b, 'c)) => unit,
  lazyMany: (unit => (string, list(arg))) => unit,

  fn1: 'a 'r. (string, ('a => 'r), 'a) => 'r,
  fn2: 'a 'b 'r. (string, (('a, 'b) => 'r), 'a, 'b) => 'r,
  fn3: 'a 'b 'c 'r. (string, (('a, 'b, 'c) => 'r), 'a, 'b, 'c) => 'r,
  fn4: 'a 'b 'c 'd 'r. (string, (('a, 'b, 'c, 'd) => 'r), 'a, 'b, 'c, 'd) => 'r,

  isEnabled: unit => bool,
  enable: unit => unit /* Too imperative? Might be removed */
};

let make: string => logger;
let enable: string => unit;
let disable: unit => unit;
let isEnabled: string => bool;
