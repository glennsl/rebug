module Format = {
  let _s = 1000.;
  let _m = _s *. 60.;
  let _h = _m *. 60.;
  let _d = _h *. 24.;

  let _format = (t, d, unit) =>
    t /. d |> Js.Math.round
           |> Js.String.make
           |> (s) => {j|$s $unit|j};

  let ms =
    fun | ms when ms >= _d => _format(ms, _d, "d")
        | ms when ms >= _h => _format(ms, _h, "h")
        | ms when ms >= _m => _format(ms, _h, "h")
        | ms when ms >= _s => _format(ms, _s, "s")
        | ms => _format(ms, 1., "ms");
};

let hash = (input) => {
  let value = ref(0);

  input |> String.iter(
           (c) => value := (value^ lsl 5) - value^ + Char.code(c));

  Js.Math.abs_int(value^)
};

let selectColor = (colors, string) => {
  let choices = Array.length(colors);
  let index = hash(string) mod choices;

  colors[index]
};

/* returns a timer function that when invoked will return the time since its
 * previous invocation */
let timer = () => {
  let prev = ref(Js.Date.now());

  () => {
    let now = Js.Date.now();
    let delta = now -. prev^;
    prev := now;
    delta
  }
};
