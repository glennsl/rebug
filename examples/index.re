let a = Debug.make("debug:a");
let b = Debug.make("debug:b");

let obj = { "foo": 42, "bar": true };

type record = { foo: int, bar: bool };
let record = { foo: 24, bar: false };
let ppRecord = ({ foo, bar }) =>
  {j|{ foo: $foo, bar: $bar }|j};

a.log1("The red cow jumps over the lazy giraffe");
b.log2("wat", obj);

let square =
  a.fn1("square", (x) => x * x);

/* Holy crap I have terrible imagination */
let powSquared =
  a.fn2("powSquared", (x, y) =>
    Js.Math.pow_int(~base=square(x), ~exp=square(y)));

Js.log(powSquared(4, 3));


a.lazy1 (fun () => "lazy");
a.lazy2 (fun () => ("lazy", obj));


b.error2("boo!", obj);

let doThings =
  b.fn2("doThings", ~pp1=ppRecord, (r, n) => {
    b.log2("foo: ", r.foo);
    powSquared(r.foo, n)
  });

doThings(record, 2);

let loggedParse = a.fn1("JSON.parse", Js.Json.parseExn);

Js.log(loggedParse({j|{ "foo": "bar" }|j}));