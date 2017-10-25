module Format = {
  let _s = 1000.;
  let _m = _s *. 60.;
  let _h = _m *. 60.;
  let _d = _h *. 24.;

  let _format t d unit =>
    t /. d |> Js.Math.round
           |> Js.String.make
           |> fun s => {j|$s $unit|j};

  let ms =
    fun | ms when ms >= _d => _format ms _d "d"
        | ms when ms >= _h => _format ms _h "h"
        | ms when ms >= _m => _format ms _h "h"
        | ms when ms >= _s => _format ms _s "s"
        | ms => _format ms 1. "ms";
};


let selectColor namespace colors => {
  let hash = ref 0;
  namespace |> String.iter (fun c => hash := ((!hash lsl 5) - !hash) + (Char.code c));
  Array.get colors ((Js.Math.abs_int !hash) mod (Array.length colors))
};
