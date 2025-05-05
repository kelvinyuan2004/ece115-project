#include "pinDefinitions.h"
#include "display.h"

void setup()
{
	pinMode(DS_PIN, OUTPUT);     // SER
	pinMode(SHCP_PIN, OUTPUT);   // SRCLK
	pinMode(STCP_PIN, OUTPUT);   // RCLK
	pinMode(NPN_PIN1, OUTPUT);  // NPN1 Control
	pinMode(NPN_PIN2, OUTPUT);  // NPN2 Control
}

void loop()
{
	delay(500);

	for (int i = 0; i < 100; i++)
	{
		unsigned long start = millis();

		while (millis() - start < 500UL)
		{
			displayDigits(DS_PIN, SHCP_PIN, STCP_PIN, NPN_PIN1, NPN_PIN2,
										i/10, i%10);
		}
		delay(5);
	}
	
	digitalWrite(NPN_PIN1, LOW);
	digitalWrite(NPN_PIN2, LOW);
}
