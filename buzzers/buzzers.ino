/* LedStripColorTester: Example Arduino sketch that lets you
 * type in a color on a PC and see it on the LED strip.
 *
 * To use this, you will need to plug an Addressable RGB LED
 * strip from Pololu into pin 12.  After uploading the sketch,
 * select "Serial Monitor" from the "Tools" menu.  In the input
 * box, type a color and press enter.
 *
 * The format of the color should be "R,G,B!" where R, G, and B
 * are numbers between 0 and 255 representing the brightnesses
 * of the red, green, and blue components respectively.
 *
 * For example, to get green, you could type:
 *   40,100,0!
 *
 * You can leave off the exclamation point if you change the
 * Serial Monitor's line ending setting to be "Newline" instead
 * of "No line ending".
 *
 * Please note that this sketch only transmits colors to the
 * LED strip after it receives them from the computer, so if
 * the LED strip loses power it will be off until you resend
 * the color.
 */

#include <PololuLedStrip.h>

// Create an ledStrip object and specify the pin it will use.
PololuLedStrip < 12 > ledStrip;

// Create a buffer for holding the colors (3 bytes per color).
#define LED_COUNT 60
rgb_color colors[LED_COUNT];

#define RED_PIN 11
#define BLUE_PIN 9
#define GREEN_PIN 10
#define YELLOW_PIN 8

#define RESET_PIN 5

#define RGB_WRITE_DELAY 10

#define LIGHT_UP_DURATION 2500

#define WAIT_FOR_MODERATOR_STATE 0
#define WAIT_FOR_PLAYER_BUTTONS_STATE 1

void setup()
{
	// initialize the pushbutton pin as an input:
	pinMode(RED_PIN, INPUT);
	pinMode(BLUE_PIN, INPUT);
	pinMode(GREEN_PIN, INPUT);
	pinMode(YELLOW_PIN, INPUT);
	send_off();
}

int blue_state = 0;
int red_state = 0;
int green_state = 0;
int yellow_state = 0;
int reset_state = 0;

int state = WAIT_FOR_MODERATOR_STATE;

void send_color(int red, int green, int blue)
{
	rgb_color color;
	color.red = red;
	color.green = green;
	color.blue = blue;

	// Update the colors buffer.
	for (uint16_t i = 0; i < LED_COUNT; i++) {
		colors[i] = color;
	}

	// Write to the LED strip.
	ledStrip.write(colors, LED_COUNT);
	delay(RGB_WRITE_DELAY);
}

void send_off()
{
	send_color(0, 0, 0);
}

void send_white()
{
	send_color(30, 30, 30);
}

void send_red()
{
	send_color(0, 100, 0);
}

void send_green()
{
	send_color(60, 0, 0);
}

void send_blue()
{
	send_color(0, 0, 100);
}

void send_yellow()
{
	send_color(40, 72, 0);
}

void read_pins()
{
	red_state = digitalRead(RED_PIN);
	green_state = digitalRead(GREEN_PIN);
	blue_state = digitalRead(BLUE_PIN);
	yellow_state = digitalRead(YELLOW_PIN);
	reset_state = digitalRead(RESET_PIN);
}

void wait_and_return_to_moderator()
{
	delay(LIGHT_UP_DURATION);
	send_off();
	state = WAIT_FOR_MODERATOR_STATE;
}

void loop()
{
	delay(1);
	read_pins();
	if (state == WAIT_FOR_MODERATOR_STATE) {
		if (reset_state == HIGH) {
			send_white();
			state = WAIT_FOR_PLAYER_BUTTONS_STATE;
		}
	} else {
		if (red_state == HIGH) {
			send_red();
			wait_and_return_to_moderator();
		} else if (green_state == HIGH) {
			send_green();
			wait_and_return_to_moderator();
		} else if (blue_state == HIGH) {
			send_blue();
			wait_and_return_to_moderator();
		} else if (yellow_state == HIGH) {
			send_yellow();
			wait_and_return_to_moderator();
		}
	}
}
