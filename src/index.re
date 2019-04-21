open Reprocessing;

type stateT = {
  shipX: float,
  rightPressed: bool,
  leftPressed: bool,
  image: imageT,
};



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
  Draw.background(Utils.color(~r=23, ~g=23, ~b=23, ~a=255), env);
  Draw.image(image, ~pos=(int_of_float(shipX), 500), env);

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
