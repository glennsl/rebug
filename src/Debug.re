module Utils = Debug__Utils;
module Browser = Debug__Browser;

module type Env = {
  let log: list('a) => unit;
  let error: list('a) => unit;
  let format: (string, string, 'a) => string;
  let group: string => unit;
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

    fn1: 'a 'r. (string, ('a => 'r), 'a) => 'r,
    fn2: 'a 'b 'r. (string, (('a, 'b) => 'r), 'a, 'b) => 'r,
    fn3: 'a 'b 'c 'r. (string, (('a, 'b, 'c) => 'r), 'a, 'b, 'c) => 'r,
    fn4: 'a 'b 'c 'd 'r. (string, (('a, 'b, 'c, 'd) => 'r), 'a, 'b, 'c, 'd) => 'r,

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

      let logIfEnabled = (message, args) =>
        instance.enabled ? log(Env.log, message, args) : ();

      let error = (message, args) =>
        log(Env.error, message, args);
      
      let fnEnter = (message, args) => {
        Env.group(message);
        logIfEnabled("->", args);
      };
      let fnExit = (ret) => {
        logIfEnabled("<-", [arg(ret)]);
        Env.groupEnd();
        ret
      };
    };
    open! Helpers;

    {
      log1: (m) => logIfEnabled(m, []),
      log2: (m, a) => logIfEnabled(m, [arg(a)]),
      log3: (m, a, b) => logIfEnabled(m, [arg(a), arg(b)]),
      log4: (m, a, b, c) => logIfEnabled(m, [arg(a), arg(b), arg(c)]),
      logMany: logIfEnabled,

      error1: (m) => error(m, []),
      error2: (m, a) => error(m, [arg(a)]),
      error3: (m, a, b) => error(m, [arg(a), arg(b)]),
      error4: (m, a, b, c) => error(m, [arg(a), arg(b), arg(c)]),
      errorMany: error,

      lazy1: (f) => instance.enabled ? log(Env.log, f(), []) : (),
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

      fn1: (m, f) => (a) => {
        fnEnter(m, [arg(a)]);
        let ret = f(a);
        fnExit(ret)
      },
      fn2: (m, f) => (a, b) => {
        fnEnter(m, [arg(a), arg(b)]);
        let ret = f(a, b);
        fnExit(ret)
      },
      fn3: (m, f) => (a, b, c) => {
        fnEnter(m, [arg(a), arg(b), arg(c)]);
        let ret = f(a, b, c);
        fnExit(ret)
      },
      fn4: (m, f) => (a, b, c, d) => {
        fnEnter(m, [arg(a), arg(b), arg(c), arg(d)]);
        let ret = f(a, b, c, d);
        fnExit(ret)
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
