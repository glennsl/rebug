module Utils = Debug__Utils;
module Browser = Debug__Browser;

module type Env = {
  let log: list('a) => unit;
  let error: list('a) => unit;
  let format: (string, string, 'a) => string;
  let group: list('a) => unit;
  let groupEnd: unit => unit;
  let save: option(string) => unit;
  let load: unit => option(string);
  let colors: array(string);
};

module Make = (Env: Env) => {
  type arg;
  external arg : 'a => arg = "%identity";

  type logger = {
    log1: string => unit,
    log2: 'a .(string, 'a) => unit,
    log3: 'a 'b .(string, 'a, 'b) => unit,
    log4: 'a 'b 'c .(string, 'a, 'b, 'c) => unit,
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

    fn1: 'a 'r. (string, ~pp1:'a => string=?, ~ppR:'r => string=?, ('a => 'r), 'a) => 'r,
    fn2: 'a 'b 'r. (string, ~pp1:'a => string=?, ~pp2:'b => string=?, ~ppR:'r => string=?, (('a, 'b) => 'r), 'a, 'b) => 'r,
    fn3: 'a 'b 'c 'r. (string, ~pp1:'a => string=?, ~pp2:'b => string=?, ~pp3:'c => string=?, ~ppR:'r => string=?, (('a, 'b, 'c) => 'r), 'a, 'b, 'c) => 'r,
    fn4: 'a 'b 'c 'd 'r. (string, ~pp1:'a => string=?, ~pp2:'b => string=?, ~pp3:'c => string=?, ~pp4:'d => string=?, ~ppR:'r => string=?, (('a, 'b, 'c, 'd) => 'r), 'a, 'b, 'c, 'd) => 'r,

    isEnabled: unit => bool,
    enable: unit => unit
  };

  type instance = {
    namespace: string,
    color: string,
    mutable enabled: bool
  };

  let instances = ref([]);
  let names = ref([]);
  let skips = ref([]);
  
  
  module Helpers = {
    /* a namespace ending in "*" will always be enabled */
    let isForceEnabled = (namespace) =>
      Js.String.(get(namespace, length(namespace))) === "*";

    let isSkipped = (namespace) =>
      skips^ |> List.exists(Js.Re.test(namespace));

    let isNamed = (namespace) =>
      names^ |> List.exists(Js.Re.test(namespace));
  };
  open Helpers;

    
  /* does not take into account manually enabled instances */
  let isEnabled = (namespace) =>
    isForceEnabled(namespace) || (!isSkipped(namespace) && isNamed(namespace));

  let make = (namespace) => {
    let timer = Utils.timer();
    let instance = {
      namespace,
      color: Utils.selectColor(Env.colors, namespace),
      enabled: isEnabled(namespace)
    };

    instances := [instance, ...instances^];

    module Helpers = {
      let log = (log, message, args) => {
        let delta = Utils.Format.ms(timer());
        let formatted = Env.format(namespace, delta, message);
        let color = "color:" ++ instance.color;
        log([arg(formatted), arg(color), arg("color: inherit"), arg(color), ...args])
      };

      let fnEnter = (message, args) => {
        let delta = Utils.Format.ms(timer());
        let formatted = Env.format(namespace, delta, message);
        let color = "color:" ++ instance.color;
        Env.group([arg(formatted), arg(color), arg("color: inherit"), arg(color), ...args]);
      };
      let fnExit = (ret) => {
        log(Env.log, "<-", [arg(ret)]);
        Env.groupEnd();
      };

      let id: 'a => string = Obj.magic;
    };
    open! Helpers;

    {
      log1: (m) => if (instance.enabled) { log(Env.log, m, []) },
      log2: (m, a) => if (instance.enabled) { log(Env.log, m, [arg(a)]) },
      log3: (m, a, b) => if (instance.enabled) { log(Env.log, m, [arg(a), arg(b)]) },
      log4: (m, a, b, c) => if (instance.enabled) { log(Env.log, m, [arg(a), arg(b), arg(c)]) },
      logMany: (m, args) => if (instance.enabled) { log(Env.log, m, args) },

      error1: (m) => log(Env.error, m, []),
      error2: (m, a) => log(Env.error, m, [arg(a)]),
      error3: (m, a, b) => log(Env.error, m, [arg(a), arg(b)]),
      error4: (m, a, b, c) => log(Env.error, m, [arg(a), arg(b), arg(c)]),
      errorMany: (m, args) => log(Env.error, m, args),

      lazy1: (f) =>
        if (instance.enabled) {
          log(Env.log, f(), [])
        },
      lazy2: (f) =>
        if (instance.enabled) {
          let (m, a) = f();
          log(Env.log, m, [arg(a)])
        },
      lazy3: (f) =>
        if (instance.enabled) {
          let (m, a, b) = f();
          log(Env.log, m, [arg(a), arg(b)])
        },
      lazy4: (f) =>
        if (instance.enabled) {
          let (m, a, b, c) = f();
          log(Env.log, m, [arg(a), arg(b), arg(c)])
        },
      lazyMany: (f) =>
        if (instance.enabled) {
          let (m, args) = f();
          log(Env.log, m, args)
        },

      fn1: (m, ~pp1=id, ~ppR=id, f) => (a) =>
        if (instance.enabled) {
          fnEnter(m, [arg(pp1(a))]);
          let ret = f(a);
          fnExit(ppR(ret));
          ret
        } else {
          f(a)
        },
      fn2: (m, ~pp1=id, ~pp2=id, ~ppR=id, f) => (a, b) =>
        if (instance.enabled) {
          fnEnter(m, [arg(pp1(a)), arg(pp2(b))]);
          let ret = f(a, b);
          fnExit(ppR(ret));
          ret
        } else {
          f(a, b)
        },

      fn3: (m, ~pp1=id, ~pp2=id, ~pp3=id, ~ppR=id, f) => (a, b, c) =>
        if (instance.enabled) {
          fnEnter(m, [arg(pp1(a)), arg(pp2(b)), arg(pp3(c))]);
          let ret = f(a, b, c);
          fnExit(ppR(ret));
          ret
        } else {
          f(a, b, c)
        },

      fn4: (m, ~pp1=id, ~pp2=id, ~pp3=id, ~pp4=id, ~ppR=id, f) => (a, b, c, d) =>
        if (instance.enabled) {
          fnEnter(m, [arg(pp1(a)), arg(pp2(b)), arg(pp3(c)), arg(pp4(d))]);
          let ret = f(a, b, c, d);
          fnExit(ppR(ret));
          ret
        } else {
          f(a, b, c, d)
        },

      isEnabled: () => instance.enabled,
      enable: () => instance.enabled = true
    }
  };


  let _resetNamespaces = () => {
    names := [];
    skips := [];
  };

  let _parseFilter = (filter) =>
    filter |> Js.String.splitByRe([%re "/[/s,]+/"])
           |> Js.Array.filter((ns) => ns !== "")
           |> Js.Array.forEach(
               (namespace) => {
                 let namespace = namespace |> Js.String.replaceByRe([%re "/\\*/g"], ".*?");
                 if (Js.String.get(namespace, 0) === "-") {
                   let namespace = namespace |> Js.String.substr(~from=1);
                   skips := [Js.Re.fromString({j|^$namespace\$|j}), ...skips^]
                 } else {
                   names := [Js.Re.fromString({j|^$namespace\$|j}), ...names^]
                 }
               });

  let enable = (filter) => {
    Env.save(Some(filter));

    _resetNamespaces();
    _parseFilter(filter);
    instances^ |> List.iter((i) => i.enabled = isEnabled(i.namespace))
  };

  let disable = () => {
    Env.save(None);

    _resetNamespaces();
    instances^ |> List.iter((i) => i.enabled = false)
  };
  
  /* initalization */
  let () =
    switch (Env.load()) {
    | Some(filter) => enable(filter)
    | None => ()
    };
};

include Make(Browser);
