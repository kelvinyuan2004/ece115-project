#include "pinDefinitions.h"
#include "display.h"
#include "sensors.h"

////////////////////////////
// timing parameters (ms) //
const int STARTUP_DELAY    = 3000;
const int DISPLAY_INTERVAL = 5;

// game
const int MAX_SCORE = 100;

enum GameState { RESET, PLAY, GAME_OVER };

struct GameContext
{
  GameState state;
  unsigned long lastDisplayTime;
  int score;
  int lives;
} game;

// forward
void enterReset();
void updateDisplayIfNeeded();
void doPlay();
void doGameOver();

void setup()
{
  Serial.begin(9600);
  Serial.println("Begin Setup");
  tone(PWM_PIN, 1000);

  initSensors();
  game.state = RESET;
  enterReset();
}

void loop()
{
  switch (game.state)
  {
    case RESET:
      game.state = PLAY;
      break;

    case PLAY:
      doPlay();
      break;

    case GAME_OVER:
      doGameOver();
      break;
  }
}

void enterReset()
{
  Serial.println(F(">> Restarting..."));
  delay(STARTUP_DELAY);
  reset();

  game.score = 0;
  game.lives = 3;
  game.lastDisplayTime  = millis();
  Serial.println(F(">> RESET: Game restarted"));
}

void updateDisplayIfNeeded()
{
  unsigned long now = millis();
  if (now - game.lastDisplayTime >= DISPLAY_INTERVAL)
  {
    displayDigits
    (
      DS_PIN, SHCP_PIN, STCP_PIN,
      NPN_PIN1, NPN_PIN2,
      game.score / 10,
      game.score % 10
    );
    game.lastDisplayTime = now;
  }
}

void doPlay() {
  updateDisplayIfNeeded();

  SensorReadings s = poll();
  if (s.irMissed)
	{
    game.lives--;
    Serial.print(F("IR miss, new lives = "));
    Serial.println(game.lives);
  }

  if (s.triggeredTCRT)
	{
    game.score = (game.score + 1) % MAX_SCORE;
    Serial.print(F("TCRT hit, new score = "));
    Serial.println(game.score);
  }

  if (s.piezoHit)
	{
    game.score = (game.score + 2) % MAX_SCORE;
    Serial.print(F("Piezo hit, new score = "));
    Serial.println(game.score);
  }

  if (game.lives <= 0)
	{
    Serial.println(F(">> PLAY -> GAME_OVER"));
    game.state = GAME_OVER;
  }
}

void doGameOver()
{
  Serial.println(F(">> GAME OVER"));
  game.state = RESET;
  enterReset();
}