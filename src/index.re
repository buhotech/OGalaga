open Reprocessing;

type stateT = {
  /* Running Status */
  gameHasStarted: bool,
  gameWasStarted: bool,
  exitStatus: bool,
  playerDead: bool,
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
  /* Media */
  enemyShipImage: imageT,
  playerShipImage: imageT,
  deadPlayerShipImage: imageT,
  starImage: imageT,
  bulletImage: imageT,
  font: fontT,
  backgroundMusic: soundT,
  haveNotPlayedSongYet: bool,
};

/* BOILERPLATE */
let setup = env => {
  Env.size(~width=600, ~height=800, env);
  {
    gameHasStarted: false,
    gameWasStarted: false,
    exitStatus: false,
    playerDead: false,
    rightPressed: false,
    leftPressed: false,
    shotBool: false,
    score: 0,
    enemyShips: [],
    bulletPositions: [],
    starPositions: [],
    shipX: float_of_int(Env.width(env) / 2 - 30),
    enemyShipImage: Draw.loadImage(~filename="assets/enemyShip.png", env),
    playerShipImage: Draw.loadImage(~filename="assets/playerShip.png", env),
    deadPlayerShipImage:
      Draw.loadImage(~filename="assets/explosion.png", env),
    starImage: Draw.loadImage(~filename="assets/playerBullet.png", env),
    bulletImage: Draw.loadImage(~filename="assets/playerBullet.png", env),
    font: Draw.loadFont(~filename="assets/fancy.fnt", ~isPixel=true, env),
    backgroundMusic: Env.loadSound("assets/b.wav", env),
    haveNotPlayedSongYet: true,
  };
};

let draw =
    (
      {
        gameHasStarted,
        gameWasStarted,
        exitStatus,
        playerDead,
        rightPressed,
        leftPressed,
        shotBool,
        score,
        enemyShips,
        bulletPositions,
        starPositions,
        shipX,
        enemyShipImage,
        playerShipImage,
        deadPlayerShipImage,
        starImage,
        bulletImage,
        font,
        backgroundMusic,
        haveNotPlayedSongYet,
      } as state,
      env,
    ) => {
  /* If the user hits 'Q' => exit window/game */
  if (exitStatus) {
    exit(0);
  };

  if (gameWasStarted && haveNotPlayedSongYet) {
    Env.playSound(backgroundMusic, ~volume=2.110, ~loop=true, env);
  };

  /* Filter out ships that have collided with a bullet */
  let enemiesNotShot =
    List.filter(
      ((enemyX, enemyY)) =>
        !
          List.exists(
            ((bulletX, bulletY)) =>
              Utils.intersectRectRect(
                (float_of_int(enemyX + 11), float_of_int(enemyY)),
                31.,
                40.,
                (float_of_int(bulletX), float_of_int(bulletY)),
                10.,
                10.,
              ),
            bulletPositions,
          ),
      enemyShips,
    );

  /* Filter out bullets that have collided with a ship */
  let strayBullets =
    List.filter(
      ((bulletX, bulletY)) =>
        !
          List.exists(
            ((enemyX, enemyY)) =>
              Utils.intersectRectRect(
                (float_of_int(enemyX + 11), float_of_int(enemyY)),
                31.,
                40.,
                (float_of_int(bulletX), float_of_int(bulletY)),
                10.,
                10.,
              ),
            enemyShips,
          ),
      bulletPositions,
    );

  /*
   * At this point, only ships that have been SHOT are DESTROYED (filter above)
   * Therefore, we can give a point for every ship destroyed
   */
  let updatedScore =
    List.length(enemyShips) > List.length(enemiesNotShot)
      ? score + 1 : score;

  /* filter out ships that hit player
   * At this point in this frame, the only enemies left on screen are
   * ships that have not gone out of bounds or been shot
   * Any other collision would be between an enemy and player ship
   */
  let enemiesNotShotAndNotCollidedWith =
    List.filter(
      ((enemyX, enemyY)) =>
        !
          Utils.intersectRectRect(
            (float_of_int(enemyX - 20), float_of_int(enemyY)),
            31.,
            40.,
            (shipX, 700.00),
            40.,
            40.,
          ),
      enemiesNotShot,
    );

  /* previous step filtered ships that hit us
   * If there are less ships since that check than a ship must have hit us
   * Therefore, we can safely assert the status of the player
   */
  let isPlayerDead =
    List.length(enemiesNotShot) > List.length(enemiesNotShotAndNotCollidedWith);

  /* Now we can FILTER out ENEMY SHIPS that are out of bounds */
  let enemiesStillOnScreen =
    List.filter(((enemyX, enemyY)) => enemyY < 800, enemiesNotShotAndNotCollidedWith);

  /* Now we can FILTER out BULLETS that are out of bounds */
  let bulletsStillOnScreen =
    List.filter(((xBullet, yBullet)) => yBullet > 0, strayBullets);

  /* Now we can FILTER out STARS that are out of bounds */
  let starPositions =
    List.filter(((starX, starY)) => starY < 800, starPositions);

  /* MOVE BULLETS UPWARD */
  let bulletPositions =
    List.map(((x, y)) => (x, y - 2), bulletsStillOnScreen);

  /* MOVE STARS DOWNWARD */
  let starPositions = List.map(((x, y)) => (x, y + 15), starPositions);

  /* MOVE SHIPS DOWNWARD */
  let enemiesWithUpdatedPositions =
    List.map(((x, y)) => (x, y + 3), enemiesStillOnScreen);

  /* respawn enemies */
  let remainingEnemies =
    List.length(enemyShips) < 12
      ? List.append(
          [
            (
              Utils.random(50, Env.width(env) - 100),
              0 - Utils.random(28, 600),
            ),
          ],
          enemiesWithUpdatedPositions,
        )
      : enemiesWithUpdatedPositions;

  /* respawning stars */
  let prettySky =
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
      : starPositions;

  /* Set new X COORDINATE for PLAYER */
  let shipCurrentX =
    rightPressed
      ? shipX +. 4.2 > float_of_int(Env.width(env))
          ? (-70.0) : shipX +. 4.20
      : leftPressed
          ? shipX -. 4.20 < (-70.0)
              ? float_of_int(Env.width(env)) : shipX -. 4.20
          : shipX;

  /* If there are less enemies, after filtering out ships that have gone out of bounds
   * then player failed to kill a ship. This is penalized with the less of a point
   */
  let pointsAfterOutOfBoundCheck =
    enemiesStillOnScreen < enemiesNotShotAndNotCollidedWith ? updatedScore - 1 : updatedScore;

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
  Draw.image(
    playerDead ? deadPlayerShipImage : playerShipImage,
    ~pos=(int_of_float(shipX), 700),
    env,
  );

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
      playerDead
        ? "GAME OVER - PRESS 'P' TO RESTART"
        : gameHasStarted
            ? string_of_int(score)
            : gameWasStarted
                ? "Press 'P' to resume or 'Q' to quit"
                : "Press 'P' to start or 'Q' to quit",
    ~pos=
      playerDead
        ? (Env.width(env) / 2 - 304, 40)
        : gameHasStarted
            ? (Env.width(env) / 2, 40) : (Env.width(env) / 2 - 240, 40),
    env,
  );

  /* NEXT GAME STATE */
  isPlayerDead
    ? {...state, playerDead: true}
    : gameHasStarted
        ? {
          ...state,
          score: pointsAfterOutOfBoundCheck,
          haveNotPlayedSongYet: false,
          shotBool: false,
          bulletPositions,
          shipX: shipCurrentX,
          enemyShips: remainingEnemies,
          starPositions: prettySky,
        }
        /* IF GAME has yet to START or has been PAUSED
         * If game PAUSED freeze PLAYER
         * If game NEVER STARTED keep PLAYER CENTERED
         */
        : {
          ...state,
          shipX:
            gameWasStarted ? shipX : float_of_int(Env.width(env) / 2 - 30),
          gameHasStarted: false,
        };
};

let keyPressed =
    (
      {
        shipX,
        bulletPositions,
        exitStatus,
        gameHasStarted,
        playerDead,
        haveNotPlayedSongYet,
      } as state,
      env,
    ) =>
  Events.(
    switch (Env.keyCode(env)) {
    | Q => {...state, exitStatus: true}
    | P =>
      playerDead
        ? {
          ...state,
          playerDead: false,
          gameHasStarted: true,
          gameWasStarted: true,
          exitStatus: false,
          rightPressed: false,
          leftPressed: false,
          shotBool: false,
          score: 0,
          enemyShips: [],
          bulletPositions: [],
          starPositions: [],
          shipX,
        }
        : {...state, gameHasStarted: !gameHasStarted, gameWasStarted: true}
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

let keyReleased = (state, env) =>
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
