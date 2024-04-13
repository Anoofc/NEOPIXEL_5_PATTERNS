#define DEBUG 0

#define STRIP_1_PIN 2
#define STRIP_2_PIN 3
#define STRIP_3_PIN 4 
#define STRIP_4_PIN 5
#define STRIP_5_PIN 6

#define STRIP_1_LED_COUNT 30
#define STRIP_2_LED_COUNT 30
#define STRIP_3_LED_COUNT 30
#define STRIP_4_LED_COUNT 30
#define STRIP_5_LED_COUNT 30

#define STRIP_1_DELAY 50
#define STRIP_2_DELAY 50
#define STRIP_3_DELAY 50
#define STRIP_4_DELAY 50
#define STRIP_5_DELAY 50

// FOR COLOR WIPE ONLY
#define STRIP_1_COLOR_1 255, 0, 0
#define STRIP_1_COLOR_2 0, 255, 0
#define STRIP_1_COLOR_3 0, 0, 255

// FOR THEATER CHASE ONLY
#define STRIP_3_COLOR_1 255, 0, 0
#define STRIP_3_COLOR_2 0, 255, 0
#define STRIP_3_COLOR_3 0, 0, 255


#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

uint32_t strip1_update_time = 0;
uint32_t strip2_update_time = 0;
uint32_t strip3_update_time = 0;
uint32_t strip4_update_time = 0;
uint32_t strip5_update_time = 0;


Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(STRIP_1_LED_COUNT, STRIP_1_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(STRIP_2_LED_COUNT, STRIP_2_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip3 = Adafruit_NeoPixel(STRIP_3_LED_COUNT, STRIP_3_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip4 = Adafruit_NeoPixel(STRIP_4_LED_COUNT, STRIP_4_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip5 = Adafruit_NeoPixel(STRIP_5_LED_COUNT, STRIP_5_PIN, NEO_GRB + NEO_KHZ800);

void colorWipe(uint8_t r, uint8_t g, uint8_t b, uint16_t wait) {
  static uint8_t i = 0;
  if (millis() - strip1_update_time < wait) {
    return;
  } 
    strip1_update_time = millis();
    strip1.setPixelColor(i, strip1.Color(r,g,b));         //  Set pixel's color (in RAM)
    strip1.show();                          //  Update strip to match
    i++;
    if (i >= STRIP_1_LED_COUNT) {
      i = 0;
    }
}

void colorWipe_Run() {
  static uint8_t mode = 0;
  if( mode == 0) {
    colorWipe(255, 0, 0, STRIP_1_DELAY);
  } else if (mode == 1) {
    colorWipe(0, 255, 0, STRIP_1_DELAY);
  } else if (mode == 2) {
    colorWipe(0, 0, 255, STRIP_1_DELAY);
  }
  mode++; if (mode > 2) { mode = 0; }
}



// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void rainbow(uint16_t wait) {
  if (millis() - strip2_update_time < wait) {
    return;
  } 
    strip2_update_time = millis();
  
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    strip2.rainbow(firstPixelHue);
    strip2.show(); // Update strip with new contents
  }
}

// THEATER CHASE

void theaterChase(uint8_t red, uint8_t green, uint8_t blue, uint32_t wait) {
  static int a = 0;
  static int b = 0;
  static int c = 0;
  static uint32_t lastUpdateTime = 0;

  if (millis() - lastUpdateTime < wait) {
    return;
  }

  lastUpdateTime = millis();

  for (uint8_t i = 0; i < 3; i++) {
    if (a < 10) {
      if (b < 3) {
        strip3.clear();
        for (c = b; c < STRIP_3_LED_COUNT; c += 3) {
          strip3.setPixelColor(c, strip3.Color(red, green, blue));
        }
        strip3.show();
        b++;
      } else { b = 0; a++; }
    } else { a = 0; }
  }
}

void theaterChase_Run() {
  static uint8_t mode = 0;
  if (mode == 0) {
    theaterChase(STRIP_3_COLOR_1, STRIP_3_DELAY);
  } else if (mode == 1) {
    theaterChase(STRIP_3_COLOR_2, STRIP_3_DELAY);
  } else if (mode == 2) {
    theaterChase(STRIP_3_COLOR_3, STRIP_3_DELAY);
  }
}

void stripBegin() {
  strip1.begin();
  strip2.begin();
  strip3.begin();
  strip4.begin();
  strip5.begin();
}

void stripClear() {
  strip1.clear();
  strip2.clear();
  strip3.clear();
  strip4.clear();
  strip5.clear();
  strip1.show();
  strip2.show();
  strip3.show();
  strip4.show();
  strip5.show();
}

void setup() {
  Serial.begin(9600);

  stripBegin();
  stripClear();


}



void loop() {

  rainbow(STRIP_2_DELAY);
  colorWipe_Run();
  theaterChase_Run();

}
