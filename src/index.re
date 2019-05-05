open Reprocessing;

type stateT = {
  gameHasStarted: bool,
  gameWasStarted: bool,
  exitStatus: bool,
  shipX: float,
  rightPressed: bool,
  leftPressed: bool,
  image: imageT,
  shotIMG: imageT,
  shotBool: bool,
  enemy_ships: list((int, int)),
  enemy_ship_image: imageT,
  star_image: imageT,
  bulletPositions: list((int, int)),
  starsPositions: list((int, int)),
  lastX: float,
  score: int,
  font: fontT,
  prompt: fontT,
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
    gameHasStarted: false,
    gameWasStarted: false,
    exitStatus: false,
    image: Draw.loadImage(~filename="assets/playerShip.png", env),
    bulletPositions: [],
    starsPositions: [],
    enemy_ship_image: Draw.loadImage(~filename="assets/enemyShip.png", env),
    star_image: Draw.loadImage(~filename="assets/playerBullet.png", env),
    shotIMG: Draw.loadImage(~filename="assets/playerBullet.png", env),
    shotBool: false,
    shipX: float_of_int(Env.width(env) / 2 - 30),
    lastX: 0.,
    score: 0,
    font: Draw.loadFont(~filename="assets/fancy.fnt", ~isPixel=true, env),
    prompt: Draw.loadFont(~filename="assets/fancy.fnt", ~isPixel=true, env),
    rightPressed: false,
    leftPressed: false,
    enemy_ships: [],
  };
};

let draw =
    (
      {
        gameHasStarted,
        gameWasStarted,
        exitStatus,
        score,
        font,
        prompt,
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
        starsPositions,
        star_image,
      } as state,
      env,
    ) => {
  if (exitStatus) {
    exit(0);
  };

  Draw.background(Utils.color(~r=0, ~g=15, ~b=25, ~a=255), env);

  List.iter(
    item =>
      Draw.pixel(
        item,
        Utils.color(
          ~r=Utils.random(0, 255),
          ~g=Utils.random(0, 255),
          ~b=Utils.random(0, 255),
          ~a=255,
        ),
        env,
      ),
    starsPositions,
  );

  List.iter(
    item => Draw.image(enemy_ship_image, ~pos=item, env),
    enemy_ships,
  );

  Draw.image(image, ~pos=(int_of_float(shipX), 700), env);

  Draw.text(
    ~font,
    ~body=
      gameHasStarted
        ? string_of_int(score)
        : gameWasStarted
            ? "Press 'P' to resume or 'Q' to quit"
            : "Press 'P' to start or 'Q' to quit",
    ~pos=
      gameHasStarted
        ? (Env.width(env) / 2, 40) : (Env.width(env) / 2 - 240, 40),
    env,
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
                    (float_of_int(xTemp + 11), float_of_int(yTemp)),
                    31.,
                    40.,
                    (float_of_int(bulletX), float_of_int(bulletY)),
                    10.,
                    10.,
                  ),
                bulletPositions,
              ),
          enemy_ships,
        ),
      enemy_ships,
    );

  let bulletPositions =
    List.filter(
      ((bulletX, bulletY)) =>
        List.exists(
          ((x, y)) =>
            !
              List.exists(
                ((xTemp, yTemp)) =>
                  Utils.intersectRectRect(
                    (float_of_int(xTemp + 11), float_of_int(yTemp)),
                    31.,
                    40.,
                    (float_of_int(bulletX), float_of_int(bulletY)),
                    10.,
                    10.,
                  ),
                enemy_ships,
              ),
          bulletPositions,
        ),
      bulletPositions,
    );

  let newScore =
    List.length(enemy_ships) > List.length(newShips) ? score + 1 : score;

  let newShips = List.filter(((xTemp, yTemp)) => yTemp < 800, newShips);

  let bulletPositions =
    List.filter(((xBullet, yBullet)) => yBullet > 0, bulletPositions);

  let starsPositions =
    List.filter(((xTemp, yTemp)) => yTemp < 800, starsPositions);

  let bulletPositions = List.map(((x, y)) => (x, y - 2), bulletPositions);

  let starsPositions = List.map(((x, y)) => (x, y + 15), starsPositions);

  let newShips = List.map(((x, y)) => (x, y + 3), newShips);

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
  gameHasStarted
    ? {
      ...state,
      score: newScore,
      shotBool: false,
      bulletPositions,
      shipX: shipCurrentX,
      lastX: lastXNew,
      enemy_ships:
        List.length(enemy_ships) < 12
          ? List.append(
              [
                (
                  Utils.random(50, Env.width(env) - 100),
                  0 - Utils.random(28, 600),
                ),
              ],
              newShips,
            )
          : newShips,
      starsPositions:
        List.length(starsPositions) < 52
          ? List.append(
              [
                (
                  Utils.random(-10, Env.width(env) + 10),
                  0 - Utils.random(28, 600),
                ),
              ],
              starsPositions,
            )
          : starsPositions,
    }
    : {
      ...state,
      shipX: gameWasStarted ? shipX : float_of_int(Env.width(env) / 2 - 30),
      gameHasStarted: false,
    };
};

let keyPressed =
    ({shipX, bulletPositions, exitStatus, gameHasStarted} as state, env) =>
  Events.(
    switch (Env.keyCode(env)) {
    | Q => {...state, exitStatus: true}
    | P => {...state, gameHasStarted: !gameHasStarted, gameWasStarted: true}
    | Right
    | D => {...state, rightPressed: true}
    | Left
    | A => {...state, leftPressed: true}
    | Space => {
        ...state,
        shotBool: true,
        bulletPositions:
          gameHasStarted
            ? List.append(
                [(int_of_float(shipX +. 34.), 700)],
                bulletPositions,
              )
            : bulletPositions,
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