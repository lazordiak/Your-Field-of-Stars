#include <Adafruit_NeoPixel.h>

#include <Time.h>
#include <TimeLib.h>

#include <CapacitiveSensor.h>


// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        6 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 5 // Popular NeoPixel ring size

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


/*
 * CapitiveSense Library Demo Sketch
 * Paul Badger 2008
 * Uses a high value resistor e.g. 10M between send pin and receive pin
 * Resistor effects sensitivity, experiment with values, 50K - 50M. Larger resistor values yield larger sensor values.
 * Receive pin is the sensor pin - try different amounts of foil/metal on this pin
 */


CapacitiveSensor   cs_4_2 = CapacitiveSensor(4,2);        // 10M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired
CapacitiveSensor   cs_5_3 = CapacitiveSensor(5,3);        // 10M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired

int state = 1;
bool touchedToday = false;
int pastDay;
int currentDay;
bool touchedThisMinute = false;

int days;
int oldDay;

int minutes;
int oldMinutes;

int seconds = 0;
int oldSeconds = 0;
int touchCounter = 0;

long initialCapReading1;
long initialCapReading2;

//counter for timer people are touching cube
int counter = 0;

void setup()                    
{
   cs_4_2.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 1 - just as an example
   cs_5_3.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 1 - just as an example
   Serial.begin(9600);

   //max that brightness out!
   pixels.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
   pixels.show();            // Turn OFF all pixels ASAP
   pixels.setBrightness(255); // Set BRIGHTNESS to about 1/5 (max = 255)

   //the time we set to is arbitrary
   setTime(0,30,0,1,0,0);

   initialCapReading1 =  cs_4_2.capacitiveSensor(30);
   initialCapReading2 =  cs_5_3.capacitiveSensor(30);
}

void loop()                    
{
  Serial.println(state);
  days = day();
  minutes = minute();
    pixels.clear(); // Set all pixel colors to 'off'

    long start = millis();
    long total1 =  cs_4_2.capacitiveSensor(30);
    long total2 = cs_5_3.capacitiveSensor(30);

    if (abs(total1 - initialCapReading1) > 5000 && !touchedThisMinute) {
      //touching = true;
      //start = millis();
      //we can estimate thirty seconds in the future when we start this
      seconds = second();
      if (seconds != oldSeconds) {
        counter += 1;
        if (counter >= 10) {
          state += 1;
          Serial.println("TEN SECONDS");
          touchedThisMinute = true;
        }
      }
    } else if (abs(total2 - initialCapReading2) > 5000 && !touchedThisMinute) {
      seconds = second();
      if (seconds != oldSeconds) {
        counter += 1;
        if (counter >= 10) {
          state += 1;
          Serial.println("TEN SECONDS");
          touchedThisMinute = true;
        }
      }
    }  else {
      counter = 0;
    }

    //if they're touching this second && they were touching the last second, increment the counter by 1
    //if they stop reset the counter

    switch (state) {
      case 0:
        pixels.clear();
        pixels.show();
        break;
      case 1:
        pixels.setPixelColor(0, pixels.Color(255, 249, 249));
        pixels.show();   // Send the updated pixel colors to the hardware.
        break;
      case 2:
         pixels.setPixelColor(0, pixels.Color(255, 249, 249));
         pixels.setPixelColor(1, pixels.Color(255, 249, 249));
         pixels.show();   // Send the updated pixel colors to the hardware.
         break;
      case 3:
         pixels.setPixelColor(0, pixels.Color(255, 249, 249));
         pixels.setPixelColor(1, pixels.Color(255, 249, 249));
         pixels.setPixelColor(2, pixels.Color(255, 249, 249));
         pixels.show();   // Send the updated pixel colors to the hardware.
         break;
      case 4:
         pixels.setPixelColor(0, pixels.Color(255, 249, 249));
         pixels.setPixelColor(1, pixels.Color(255, 249, 249));
         pixels.setPixelColor(2, pixels.Color(255, 249, 249));
         pixels.setPixelColor(3, pixels.Color(255, 249, 249));
         pixels.show();   // Send the updated pixel colors to the hardware.
         break;
      case 5:
         pixels.setPixelColor(0, pixels.Color(255, 249, 249));
         pixels.setPixelColor(1, pixels.Color(255, 249, 249));
         pixels.setPixelColor(2, pixels.Color(255, 249, 249));
         pixels.setPixelColor(3, pixels.Color(255, 249, 249));
         pixels.setPixelColor(4, pixels.Color(255, 249, 249));
         pixels.show();   // Send the updated pixel colors to the hardware.
         break;
    }

    delay(10);                             // arbitrary delay to limit data to serial port 

    /*if (oldDay != days) {
      if (!touchedToday) {
        state -= 1;
      } else {
        touchedToday = false;
      }
    }*/

    if (oldMinutes != minutes) {
      if (!touchedThisMinute) {
        state -= 1;
      } else {
        touchedThisMinute = false;
      }
    }

    oldSeconds = seconds;
    oldDay = days;
    oldMinutes = minutes;

    Serial.println(minutes);
}
