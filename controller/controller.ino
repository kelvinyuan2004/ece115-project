#include "pinDefinitions.h"
#include "display.h"

int score;
bool scoreUpdated;

unsigned long startTime;
unsigned long lastEdgeTime;
unsigned long lastPoll;
unsigned long lastDisplayUpdate;

const int startupDelay = 1000; // startup delay for polling system
const int pollRate = 50; // polling rate for visual sensor (ms)
const int edgeBuffer = 20; // buffer for the edge updates

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

	// setup initial states, variables/, misc.
	score = 0;
	scoreUpdated = 0;

	startTime = millis();
	lastEdgeTime = millis();
	lastPoll = millis();
	Serial.begin(9600);
}

void loop()
{
	visualSensor();
}

void visualSensor()
{
	unsigned long currentTime = millis();

	if (currentTime - startTime < startupDelay) return;
	
	if (currentTime - lastDisplayUpdate >= 5)
	{
		displayDigits(DS_PIN, SHCP_PIN, STCP_PIN, NPN_PIN1, NPN_PIN2, score/10, score%10);
		lastDisplayUpdate = currentTime;
	}

	if (currentTime - lastPoll >= pollRate)
	{
    	noInterrupts();
    	unsigned long timeSinceLastEdge = currentTime - lastEdgeTime;
    	interrupts();

		if(timeSinceLastEdge > edgeBuffer)
			if (scoreUpdated)
				scoreUpdated = 0;
			else
				updateScore();

		lastPoll = currentTime;
	}
}

void edgeDetected()
{
  lastEdgeTime = millis();
}

void updateScore()
{
	score += 1;
	if (score > 99) score = 0;				
	Serial.println("Score updated: " + score);
}
