
type logger 'a = 'a => unit;

let make : string => logger 'a;
let enable : string => unit;
let disable : unit => unit;
let isEnabled : string => bool;