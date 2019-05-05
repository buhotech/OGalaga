open Reprocessing;

type stateT = {
  /* Running Status */
  gameHasStarted: bool,
  gameWasStarted: bool,
  exitStatus: bool,
  /* User Activities */
  rightPressed: bool,
  leftPressed: bool,
  shotBool: bool,
  score: int,
  /* Actor Locations */
  enemyShips: list((int, int)),
  bulletPositions: list((int, int)),
  starPositions: list((int, int)),
  shipX: float,
  lastX: float,
  /* Media */
  enemyShipImage: imageT,
  playerShipImage: imageT,
  starImage: imageT,
  bulletImage: imageT,
  font: fontT,
  backgroundMusic: soundT,
};

/* BOILERPLATE */
let setup = env => {
  Env.size(~width=600, ~height=800, env);
  {
    gameHasStarted: false,
    gameWasStarted: false,
    exitStatus: false,
    rightPressed: false,
    leftPressed: false,
    shotBool: false,
    score: 0,
    enemyShips: [],
    bulletPositions: [],
    starPositions: [],
    shipX: float_of_int(Env.width(env) / 2 - 30),
    lastX: 0.,
    enemyShipImage: Draw.loadImage(~filename="assets/enemyShip.png", env),
    playerShipImage: Draw.loadImage(~filename="assets/playerShip.png", env),
    starImage: Draw.loadImage(~filename="assets/playerBullet.png", env),
    bulletImage: Draw.loadImage(~filename="assets/playerBullet.png", env),
    font: Draw.loadFont(~filename="assets/fancy.fnt", ~isPixel=true, env),
    backgroundMusic: Env.loadSound("assets/b.wav", env),
  };

};

let draw =
    (
      {
        gameHasStarted,
        gameWasStarted,
        exitStatus,
        rightPressed,
        leftPressed,
        shotBool,
        score,
        enemyShips,
        bulletPositions,
        starPositions,
        shipX,
        lastX,
        enemyShipImage,
        playerShipImage,
        starImage,
        bulletImage,
        font,
        backgroundMusic,
      } as state,
      env,
    ) => {
  /* If the user hits 'Q' => exit window/game */
  if (exitStatus) {
    exit(0);
  };
// //

 Env.playSound(backgroundMusic, env);
  /* Paint background */
  Draw.background(Utils.color(~r=0, ~g=15, ~b=25, ~a=255), env);

  /* Draw stars */
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
    starPositions,
  );

  /* Draw enemies */
  List.iter(item => Draw.image(enemyShipImage, ~pos=item, env), enemyShips);

  /* Draw Player */
  Draw.image(playerShipImage, ~pos=(int_of_float(shipX), 700), env);

  /* Draw bullets */
  List.iter(
    ((x, y)) => Draw.image(bulletImage, ~pos=(x, y - 1), env),
    bulletPositions,
  );

  /*
   * Print SCORE if game has started
   * Print START PROMPT if player has yet to start game
   * Print RESUME PROMPT if game is pauseed
   */
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

  /* Filter out ships that have collided with a bullet */
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
          enemyShips,
        ),
      enemyShips,
    );

  /* Filter out bullets that have collided with a ship */
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
                enemyShips,
              ),
          bulletPositions,
        ),
      bulletPositions,
    );

  /*
   * At this point, only ships that have been SHOT are DESTROYED (filter above)
   * Therefore, we can give a point for every ship destroyed
   */
  let newScore =
    List.length(enemyShips) > List.length(newShips) ? score + 1 : score;

  /* Now we can FILTER out ENEMY SHIPS that are out of bounds */
  let newShips = List.filter(((xTemp, yTemp)) => yTemp < 800, newShips);

  /* Now we can FILTER out BULLETS that are out of bounds */
  let bulletPositions =
    List.filter(((xBullet, yBullet)) => yBullet > 0, bulletPositions);

  /* Now we can FILTER out STARS that are out of bounds */
  let starPositions =
    List.filter(((xTemp, yTemp)) => yTemp < 800, starPositions);

  /* MOVE BULLETS UPWARD */
  let bulletPositions = List.map(((x, y)) => (x, y - 2), bulletPositions);

  /* MOVE STARS DOWNWARD */
  let starPositions = List.map(((x, y)) => (x, y + 15), starPositions);

  /* MOVE SHIPS DOWNWARD */
  let newShips = List.map(((x, y)) => (x, y + 3), newShips);

  /* Set new X COORDINATE for PLAYER */
  let shipCurrentX =
    rightPressed
      ? shipX +. 4.2 > float_of_int(Env.width(env))
          ? (-70.0) : shipX +. 4.20
      : leftPressed
          ? shipX -. 4.20 < (-70.0)
              ? float_of_int(Env.width(env)) : shipX -. 4.20
          : shipX;

  /* NEXT GAME STATE */
  gameHasStarted
    ? {
      ...state,
      score: newScore,
      shotBool: false,
      bulletPositions,
      shipX: shipCurrentX,
      enemyShips:
        List.length(enemyShips) < 12
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
      starPositions:
        List.length(starPositions) < 52
          ? List.append(
              [
                (
                  Utils.random(-10, Env.width(env) + 10),
                  0 - Utils.random(28, 600),
                ),
              ],
              starPositions,
            )
          : starPositions,
    }
    /* IF GAME has yet to START or has been PAUSED
     * If game PAUSED freeze PLAYER
     * If game NEVER STARTED keep PLAYER CENTERED
     */
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
