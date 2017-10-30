let a = Debug.make("debug:a");
let b = Debug.make("debug:b");

let obj = { "foo": 42, "bar": true };

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