let debug = Debug.make("my:namespace");

let () =
  debug.log2("wat", { "foo": 42 });

let () =
  debug.log("bar");

let () =
  debug.logMany("msg", Debug.[arg(1), arg("foo"), arg({"bar": 98})]);
