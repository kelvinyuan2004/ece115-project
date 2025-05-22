#include "Sensors.h"
#include "pinDefinitions.h"

//——sensor timing constants——
static const unsigned long POLL_RATE = 50; // ms
static const unsigned long EDGE_BUFFER = 20; // ms
static const unsigned long DEBOUNCE = 20000; // us
static const int PIEZO_THRESHOLD = 100; // analog units

//——polled states——
namespace {
  volatile bool edgeDetected    = false;
  volatile bool reflectDetected = false;
  unsigned long lastEdgeTime    = 0;
  unsigned long lastReflectMicros = 0;
  bool piezoWasAbove            = false;

  // —— moved out of poll() so reset() can clear them:
  unsigned long lastPoll        = 0;
  bool         irMissCounted    = true;
}

// IR beam ISR
static void IR_ISR() 
{
  edgeDetected = true;
}

// TCRT reflect ISR
static void TCRT_ISR()
{  
	unsigned long now = micros();
  
	if (now - lastReflectMicros >= DEBOUNCE)
	{
		lastReflectMicros = now;
		reflectDetected  = true;
	}
}

void initSensors()
{
	// IR sensor
	pinMode(VIS_IN_PIN, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(VIS_IN_PIN), IR_ISR, CHANGE);
	lastEdgeTime = millis();

	// TCRT sensor
	pinMode(TCRT_IN_PIN, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(TCRT_IN_PIN), TCRT_ISR, FALLING);
	lastReflectMicros = micros();

	// Piezo
	pinMode(PIEZO_IN_PIN, INPUT);
}


void reset()
{
  noInterrupts();
  edgeDetected      = false;
  reflectDetected   = false;
	piezoWasAbove     = (analogRead(PIEZO_IN_PIN) > PIEZO_THRESHOLD);
  irMissCounted     = true;

  unsigned long nowMs = millis();
  lastEdgeTime       = nowMs;
  lastPoll           = nowMs;

  lastReflectMicros  = micros();
  interrupts();
}

SensorReadings poll()
{
	SensorReadings r = { false, false, false, 0 };
	unsigned long now = millis();

	//——IR miss detection——  
	static unsigned long lastPoll = 0;
	if (now - lastPoll >= POLL_RATE)
	{
		lastPoll = now;
		
		if (edgeDetected)
		{
			lastEdgeTime = now;
			edgeDetected = false;
		}
		
		bool rawMiss = (now - lastEdgeTime > EDGE_BUFFER);
  	
		if (rawMiss && !irMissCounted)
		{
      // Only once per break
      r.irMissed = true;
    	irMissCounted = true;
    }
    else if (!rawMiss)
		{
      // Beam has re-opened
      irMissCounted = false;
    }
	}

	//——TCRT reflection——  
	if (reflectDetected)
	{
		r.triggeredTCRT = true;
		reflectDetected = false;
	}

	//——Piezo hit——
	int piezoData = analogRead(PIEZO_IN_PIN);
	r.piezoValue = piezoData;
	
	if (piezoData > PIEZO_THRESHOLD && !piezoWasAbove)
	{
		r.piezoHit = true;
		piezoWasAbove = true;
	}
	else if (piezoData <= PIEZO_THRESHOLD)
	{
		piezoWasAbove = false;
	}

	return r;
}