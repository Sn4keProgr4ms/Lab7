#define BUTTONCODE "buttonpushed"
#define BUTTONPIN V1
#define TEMPPIN A4
#define CELSIUSPIN V3
#define FAHRENHEITPIN V2
#define MAXVOLTAGE 3.3

#include "env.h"
#include <blynk.h>
#include <oled-wing-adafruit.h>

uint8_t foo[5] = {0, 14, 2, 3, 1};

SYSTEM_THREAD(ENABLED)

OledWingAdafruit display;

int buttonPushed = 1;

void setup()
{
	Serial.begin(9600);

	uint8_t total = 0;
	int i = 0;
	while (i < 5)
	{
		total += foo[i];
		Serial.println(total);
		i++;
	}

	Serial.println(total / 5.0);

	Blynk.begin(BLYNK_AUTH_TOKEN);

	pinMode(TEMPPIN, INPUT);
	pinMode(FAHRENHEITPIN, OUTPUT);
	pinMode(CELSIUSPIN, OUTPUT);
	pinMode(BUTTONPIN, INPUT_PULLUP);

	display.setup();
	display.clearDisplay();
	display.display();
	display.setTextSize(2);
}

void loop()
{
	display.loop();
	Blynk.run();

	if (display.pressedA())
	{
		Blynk.logEvent("push_notifications");
		Blynk.begin(BLYNK_AUTH_TOKEN);

		uint16_t reading = analogRead(TEMPPIN);
		double voltage = (reading * MAXVOLTAGE) / 4095.0;
		double temperature = (voltage - 0.5) * 100;
		int tempCelsius = temperature;
		int tempFahrenheit = (((temperature) * (9.0 / 5.0)) + 32);

		Blynk.virtualWrite(CELSIUSPIN, tempCelsius);
		Blynk.virtualWrite(FAHRENHEITPIN, tempFahrenheit);

		if (temperature = 25)
		{
			Blynk.notify("High temp detected!");
		}
	}
}

BLYNK_WRITE(V1)
{

	Serial.println("Button pushed!");
	if (param.asInt() == 1)
	{
		Blynk.logEvent("push_notifications", "Hey, Blynk push here!");
	}
}
