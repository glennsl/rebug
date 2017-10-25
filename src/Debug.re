module Utils = Debug__Utils;
module Browser = Debug__Browser;

module type Env = {
  let log: 'a => string => string => string => unit;
  let format: string => string => 'a => string;
  let save: string => unit;
  let load: unit => string;
  let colors: array string;
};

module Make(Env: Env) => {

  type instance = {
    namespace: string,
    color: string,
    mutable enabled: bool
  };

  let instances = ref [];
  let names = ref [];
  let skips = ref [];

  let make namespace => {
    let prevTime = ref (Js.Date.now ());

    let instance = {
      namespace,
      color: Utils.selectColor namespace Env.colors,
      enabled: false
    };

    instances := [instance, ...!instances];

    fun payload => {
      let curr = Js.Date.now ();
      let diff = curr -. !prevTime;
      prevTime := curr;

      let formatted = Env.format namespace (Utils.Format.ms diff) payload;
      let color = "color:" ^ instance.color;
      Env.log formatted color "color: inherit" color;
    };
  };

  /* Not quite sure what the logic is behind this... */
  let _endsWithWildcard namespace =>
    Js.String.get namespace (Js.String.length namespace) === "*";

  let _isSkipped namespace =>
    !skips |> List.exists (fun re => re |> Js.Re.test namespace);

  let _isNamed namespace =>
    !names |> List.exists (fun re => re |> Js.Re.test namespace);

  let isEnabled namespace =>
    _endsWithWildcard namespace ||
    (not (_isSkipped namespace) && _isNamed namespace);

  let enable namespaces => {
    /*save namespaces;*/

    names := [];
    skips := [];

    namespaces |> Js.String.splitByRe [%re "/[/s,]+/"]
               |> Js.Array.filter (fun ns => ns !== "")
               |> Js.Array.forEach (
                  fun namespace => {
                    let namespace = namespace |> Js.String.replaceByRe [%re "/`*/g"] ".*?";
                    if (Js.String.get namespace 0 === "-") {
                      let namespace = namespace |> Js.String.substr from::1;
                      skips := [Js.Re.fromString {j|^$namespace\$|j}, ...!skips];
                    } else {
                      names := [Js.Re.fromString {j|^$namespace\$|j}, ...!names];
                    }
                  });

    !instances |> List.iter (fun i => i.enabled = isEnabled(i.namespace))
  };
  
  let disable () =>
    enable "";
};

include Make Browser;