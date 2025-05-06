#include "pinDefinitions.h"
#include "display.h"

unsigned long lastEdgeTime;
int score;
bool hasCounted;
unsigned long startupDelay;
unsigned long startTime;
unsigned long lastDisplaySwitch;

void setup()
{
	// sensor
	pinMode(VIS_IN_PIN, INPUT);   // visual sensor in
	attachInterrupt(digitalPinToInterrupt(VIS_IN_PIN), edgeDetected, FALLING);
	
	pinMode(VIS_OUT_PIN, OUTPUT); // visual sensor out
	tone(VIS_OUT_PIN, 1000);

	// display
	pinMode(DS_PIN, OUTPUT);      // SER
	pinMode(SHCP_PIN, OUTPUT);    // SRCLK
	pinMode(STCP_PIN, OUTPUT);    // RCLK
	pinMode(NPN_PIN1, OUTPUT);    // NPN1 Control
	pinMode(NPN_PIN2, OUTPUT);    // NPN2 Control

	// setup variables/misc
	Serial.begin(9600);

	startTime = millis();
	lastEdgeTime = 0;
	score = 0;
	hasCounted = 0;
	startupDelay = 1000;
	lastDisplaySwitch = 0;
}

void loop()
{
	static unsigned long lastCheck = 0;
	unsigned long currentTime = millis();

	if (currentTime - lastDisplaySwitch > 5)
	{
		displayDigits(DS_PIN, SHCP_PIN, STCP_PIN, NPN_PIN1, NPN_PIN2, score/10, score%10);
    lastDisplaySwitch = currentTime;
  }

	if (currentTime - startTime < startupDelay) return;

  if (currentTime - lastCheck > 50) {
    noInterrupts();
    unsigned long timeSinceLastEdge = currentTime - lastEdgeTime;
    interrupts();

    if (timeSinceLastEdge > 20) {
      if (!hasCounted) {
        score += 1;
        if (score > 99) score = 0;
        Serial.print("Ball Go Through! Score: ");
        Serial.println(score);
        hasCounted = true;
      }
    } else {
      hasCounted = false;
    }

    lastCheck = currentTime;
  }
}

void edgeDetected()
{
	Serial.println("Fall");
	lastEdgeTime = millis();
}
