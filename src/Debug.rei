type arg;
external arg : 'a => arg = "%identity";

type logger = {
  log: string => unit,
  log2: 'a. string => 'a => unit,
  log3: 'a 'b. string => 'a => 'b => unit,
  log4: 'a 'b 'c. string => 'a => 'b => 'c => unit,
  logMany: string => list arg => unit,
  isEnabled : unit => bool,
  enable : unit => unit
};

let make : string => logger;
let enable : string => unit;
let disable : unit => unit;
let isEnabled : string => bool;