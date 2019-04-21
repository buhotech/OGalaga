open Reprocessing;

type stateT = {
  shipX: float,
  rightPressed: bool,
  leftPressed: bool,
  /* xOffset: float, */
  /* running: runningT, */
  image: imageT,
  /* font: fontT, */
  /* score: int */
};

/* drawStars = (state, env) => {

} */

/* let drawShip = ({shipX} as state, env) => {
  if (Env.keyPressed(Right, env))
    float_of_int(int_of_float(shipX +. 60.0) mod Env.width(env));
  if (Env.keyPressed(Left, env )&& int_of_float(shipX -. 60.0) < 0))
    Env.width(env);
  shipX
} */

/* BOILERPLATE */

let setup = (env) => {
  Env.size(~width=600, ~height=600, env);
  {
    image: Draw.loadImage(~filename="playerShip.png", env),
    shipX: 260.0,
    rightPressed: false,
    leftPressed: false
  };
}

let draw = ({image, shipX, rightPressed, leftPressed} as state, env) => {
  /* print_float(shipX); */
  Draw.background(Utils.color(~r=23, ~g=23, ~b=23, ~a=255), env);
  /* Draw.fill(Utils.color(~r=41, ~g=166, ~b=244, ~a=255), env); */
  /* Draw.rect(~pos=(150, 150), ~width=300, ~height=300, env); */
  Draw.image(image, ~pos=(int_of_float(shipX), 500), env);
  /* drawStars(state, env); */
  /* drawShip(state,env);
  state */
  /* {
    ...state,
    shipX:
      /* if RIGHT */
      Env.keyPressed(Right, env)
        ? (shipX +. 10.) > float_of_int(Env.width(env))
          ? -70.0
          : shipX +. 10.0
        /* if LEFT */
        : Env.keyPressed(Left, env)
          /* if OUT OF BOUNDS on LEFT */
          ? (shipX -. 10.0) < -70.0
            ? float_of_int(Env.width(env))
            : shipX -. 10.0
          : shipX
  } */
  if (rightPressed){
    {...state, shipX: (shipX +. 10.) > float_of_int(Env.width(env)) ? -70.0 : shipX +. 10.0}
  }
  else if (leftPressed){
    {...state, shipX: (shipX -. 10.0) < -70.0 ? float_of_int(Env.width(env)) : shipX -. 10.0}
  }
  else{
    state
  }
};

/* let keyPressed = ({shipX} as state, env) =>
  Events.(
    switch (Env.keyCode(env)) {
    | Right => {...state, shipX: (shipX +. 10.) > float_of_int(Env.width(env)) ? -70.0 : shipX +. 10.0}
    | Left => {...state, shipX: (shipX -. 10.0) < -70.0 ? float_of_int(Env.width(env)) : shipX -. 10.0}
    | _ => state
    }
  ); */

let keyPressed = ({shipX} as state, env) =>
  Events.(
    switch (Env.keyCode(env)) {
    | Right
    | D => {...state, rightPressed:true}
    | Left
    | A => {...state, leftPressed:true}
    | _ => state
    }
  );

let keyReleased = ({shipX} as state, env) =>
 Events.(
    switch (Env.keyCode(env)) {
    | Right
    | D => {...state, rightPressed:false}
    | Left
    | A => {...state, leftPressed:false}
    | _ => state
    }
  );

run(~setup, ~draw, ~keyPressed, ~keyReleased, ());
