open Reprocessing;

type stateT = {
  shipX: float,
  rightPressed: bool,
  leftPressed: bool,
  image: imageT,
  shotIMG: imageT,
  shotBool: bool,
  enemy_ships: list((int, int)),
  enemy_ship_image: imageT,
  bulletPositions: list((int, int)),
  lastX: float,
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
let setup = env => {
  Env.size(~width=600, ~height=800, env);
  {
    image: Draw.loadImage(~filename="playerShip.png", env),
    bulletPositions: [],
    enemy_ship_image: Draw.loadImage(~filename="enemyShip.png", env),
    shotIMG: Draw.loadImage(~filename="playerBullet.jpeg", env),
    shotBool: false,
    shipX: 260.0,
    lastX: 0.,
    rightPressed: false,
    leftPressed: false,
    enemy_ships: [
      (Utils.random(0, Env.width(env)),(0-(Utils.random(28, 48)))),
      (Utils.random(0, Env.width(env)),(0-(Utils.random(28, 48)))),
      (Utils.random(0, Env.width(env)),(0-(Utils.random(28, 48)))),
      (Utils.random(0, Env.width(env)),(0-(Utils.random(28, 48)))),
      (Utils.random(0, Env.width(env)),(0-(Utils.random(28, 48)))),
      (Utils.random(0, Env.width(env)),(0-(Utils.random(28, 48)))),
    ],
  };
};

let draw =
    (
      {
        image,
        shipX,
        lastX,
        rightPressed,
        leftPressed,
        enemy_ships,
        enemy_ship_image,
        shotBool,
        shotIMG,
        bulletPositions,
      } as state,
      env,
    ) => {
  Draw.background(Utils.color(~r=23, ~g=45, ~b=70, ~a=255), env);
  Draw.image(image, ~pos=(int_of_float(shipX), 700), env);
  List.iter(
    item => Draw.image(enemy_ship_image, ~pos=item, env),
    enemy_ships,
  );

  let newShips =
    List.filter(
      ((xTemp, yTemp)) =>
        List.exists(
          ((x, y)) =>
            !
              List.exists(
                ((bulletX, bulletY)) =>
                  Utils.intersectRectRect(
                    (float_of_int(xTemp + 31), float_of_int(yTemp)),
                    31.,
                    100.,
                    (float_of_int(bulletX), float_of_int(bulletY)),
                    20.,
                    20.,
                  ),
                bulletPositions,
              ),
          enemy_ships,
        ),
      enemy_ships,
    );

    let newShips = List.filter(((xTemp, yTemp)) => (yTemp < 800), newShips);

  let bulletPositions = List.map(((x, y)) => (x, y - 2), bulletPositions);

let newShips = List.map(((x, y)) => (x, y + 2), newShips);

  List.iter(
    ((x, y)) => Draw.image(shotIMG, ~pos=(x, y - 1), env),
    bulletPositions,
  );

  let shipCurrentX =
    rightPressed
      ? shipX +. 4.2 > float_of_int(Env.width(env))
          ? (-70.0) : shipX +. 4.20
      : leftPressed
          ? shipX -. 4.20 < (-70.0)
              ? float_of_int(Env.width(env)) : shipX -. 4.20
          : shipX;
  let lastXNew = shotBool ? shipX : lastX;
  /*
   -->add a new (x,y) bullet List
   -->iter if y > height
   ------>remove front element
   */
  {
    ...state,
    shotBool: false,
    bulletPositions,
    shipX: shipCurrentX,
    lastX: lastXNew,
    enemy_ships:
      List.length(enemy_ships) < 6
        ? List.append([(Utils.random(0, Env.width(env)), (0-(Utils.random(28, 48))))], newShips)
        : newShips,
  };
};

let keyPressed = ({shipX, bulletPositions} as state, env) =>
  Events.(
    switch (Env.keyCode(env)) {
    | Right
    | D => {...state, rightPressed: true}
    | Left
    | A => {...state, leftPressed: true}
    | Space => {
        ...state,
        shotBool: true,
        bulletPositions:
          List.append(
            [(int_of_float(shipX +. 34.), 700)],
            bulletPositions,
          ),
      }
    | _ => state
    }
  );

let keyReleased = ({shipX} as state, env) =>
  Events.(
    switch (Env.keyCode(env)) {
    | Right
    | D => {...state, rightPressed: false}
    | Left
    | A => {...state, leftPressed: false}
    | _ => state
    }
  );

run(~setup, ~draw, ~keyPressed, ~keyReleased, ());