module Utils = Debug__Utils;
module Browser = Debug__Browser;

module type Env = {
  let log: list 'a => unit;
  let format: string => string => 'a => string;
  let save: option string => unit;
  let load: unit => option string;
  let colors: array string;
};

module Make(Env: Env) => {
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

  type instance = {
    namespace: string,
    color: string,
    mutable enabled: bool
  };

  let instances = ref [];
  let names = ref [];
  let skips = ref [];

  /* a namespace ending in "*" will always be enabled */
  let _isForceEnabled namespace =>
    Js.String.get namespace (Js.String.length namespace) === "*";

  let _isSkipped namespace =>
    !skips |> List.exists (fun re => re |> Js.Re.test namespace);

  let _isNamed namespace =>
    !names |> List.exists (fun re => re |> Js.Re.test namespace);

  /* does not take into account manually enabled instances */
  let isEnabled namespace =>
    _isForceEnabled namespace ||
    (not (_isSkipped namespace) && _isNamed namespace);


  let make namespace => {
    let prevTime = ref (Js.Date.now ());

    let instance = {
      namespace,
      color: Utils.selectColor namespace Env.colors,
      enabled: isEnabled namespace
    };

    instances := [instance, ...!instances];

    let log message args => {
      if (instance.enabled) {
        let curr = Js.Date.now ();
        let diff = curr -. !prevTime;
        prevTime := curr;

        let formatted = Env.format namespace (Utils.Format.ms diff) message;
        let color = "color:" ^ instance.color;
        Env.log [arg formatted, arg color, arg "color: inherit", arg color, ...args];
      }
    };

    {
      log: fun m => log m [],
      log2: fun m a => log m [arg a],
      log3: fun m a b => log m [arg a, arg b],
      log4: fun m a b c => log m [arg a, arg b, arg c],
      logMany: log,
      isEnabled: fun () => instance.enabled,
      enable: fun () => instance.enabled = true
    };
  };

  let _resetNamespaces () => {
    names := [];
    skips := [];
  };

  let enable namespaces => {
    Env.save (Some namespaces);

    _resetNamespaces ();

    namespaces |> Js.String.splitByRe [%re "/[/s,]+/"]
               |> Js.Array.filter (fun ns => ns !== "")
               |> Js.Array.forEach (
                  fun namespace => {
                    let namespace = namespace |> Js.String.replaceByRe [%re "/\*/g"] ".*?";
                    if (Js.String.get namespace 0 === "-") {
                      let namespace = namespace |> Js.String.substr from::1;
                      skips := [Js.Re.fromString {j|^$namespace\$|j}, ...!skips];
                    } else {
                      names := [Js.Re.fromString {j|^$namespace\$|j}, ...!names];
                    }
                  });

    !instances |> List.iter (fun i => i.enabled = isEnabled(i.namespace))
  };
  
  let disable () => {
    Env.save None;

    _resetNamespaces ();
    !instances |> List.iter (fun i => i.enabled = false)
  };

  let () =
    switch (Env.load ()) {
    | Some  namespaces => enable namespaces
    | None => ()
    };
};

include Make Browser;