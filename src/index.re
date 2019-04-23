open Reprocessing;

type stateT = {
  shipX: float,
  rightPressed: bool,
  leftPressed: bool,
  image: imageT,
  enemy_ships: list(float)
};


/**
How to spawn things??
  Draw at location with utils.random



spawning rectangles in flappy bird ~31:35


Spawn ships at random location
Spawn star  at random location
Spawn shot   


  List.iter(((x)=>{
    createEnemy
  }), enemy_ships);

*/

/* BOILERPLATE */


let createEnemy= (env) =>{
  let im =  Draw.loadImage(~filename="enemyShip.png", env);
  Draw.image(im, ~pos=(Utils.random(0, Env.width(env)),10), env)  
} 

let setup = (env) => {
  Env.size(~width=600, ~height=600, env);
  {
    image: Draw.loadImage(~filename="playerShip.png", env),
    shipX: 260.0,
    rightPressed: false,
    leftPressed: false,
    enemy_ships: [(0.),(0.),(0.)]
  };
  createEnemy(env)
}


let draw = ({image, shipX, rightPressed, leftPressed, enemy_ships} as state, env) => {
  Draw.background(Utils.color(~r=23, ~g=45, ~b=70, ~a=255), env);
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
