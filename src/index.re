open Reprocessing;

type stateT = {
  shipX: float,
  rightPressed: bool,
  leftPressed: bool,
  image: imageT,
  shotIMG: imageT,
  shotBool: bool,
  enemy_ships: list((int, int)),
  enemy_ship: imageT
};

/**
How to spawn things??
  Draw at location with utils.random

spawning rectangles in flappy bird ~31:35

Spawn ships at random location
Spawn star  at random location
Spawn shot   

*/

/* BOILERPLATE */
let setup = (env) => {
  Env.size(~width=600, ~height=600, env);
  {
    image: Draw.loadImage(~filename="playerShip.png", env),
    enemy_ship: Draw.loadImage(~filename="enemyShip.png", env),
    shotIMG:  Draw.loadImage(~filename="playerBullet.jpeg", env),
    shotBool: false,
    shipX: 260.0,
    rightPressed: false,
    leftPressed: false,
    enemy_ships: [(Utils.random(0, Env.width(env)), 10),(Utils.random(0, Env.width(env)), 10),(Utils.random(0, Env.width(env)), 10)]
  };
}

let draw = ({image, shipX, rightPressed, leftPressed, enemy_ships, enemy_ship, shotBool, shotIMG} as state, env) => {
  Draw.background(Utils.color(~r=23, ~g=45, ~b=70, ~a=255), env);
  Draw.image(image, ~pos=(int_of_float(shipX), 500), env);
  List.iter(item => Draw.image(enemy_ship, ~pos=(item), env), enemy_ships)
  if (shotBool){
     Draw.image(shotIMG, ~pos=(int_of_float(shipX), 500), env);

  }
   
  if (rightPressed){
    {...state, shipX: (shipX +. 4.2) > float_of_int(Env.width(env)) ? -70.0 : shipX +. 4.20}
  }
  else if (leftPressed){
    {...state, shipX: (shipX -. 4.20) < -70.0 ? float_of_int(Env.width(env)) : shipX -. 4.20}
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
    | Space => {...state, shotBool:true}
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
