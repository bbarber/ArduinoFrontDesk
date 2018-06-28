#include <Adafruit_NeoPixel.h>

#define STRIP1_PIN 3
#define STRIP2_PIN 4

const int buttonPin = 2;
int lightMode = 0;
int numModes = 5;

int interrupted = 0;
long debouncing_time = 200;
volatile unsigned long last_micros;

#define NUM_PIXELS 120

Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(NUM_PIXELS, STRIP1_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(NUM_PIXELS, STRIP2_PIN, NEO_GRB + NEO_KHZ800);

uint32_t DPLColor1 = strip1.Color(0, 50, 0);
uint32_t DPLColor2 = strip2.Color(15, 15, 15);

void setup() {
  Serial.begin(9600);
  
  pinMode(buttonPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonPin), debounceInterrupt, RISING);
  
  strip1.begin();
  strip2.begin();
  
  strip1.show();
  strip2.show();

  delay(100);
}

void debounceInterrupt() {
  // Todd-click prevention
  if((long)(micros() - last_micros) >= debouncing_time * 1000) {
    changeMode();
    last_micros = micros();
  }
}

void changeMode() {
  lightMode = (lightMode + 1) % numModes;
  interrupted = 1;
}

void loop() {
  interrupted = 0;
  Serial.println(lightMode);
  showMode(lightMode);
  delay(20);
}

void showMode(int mode) {
  
  switch(mode) {
    case 0:
      standard();
      break;
    case 1:
      standardFade();
      break;
    case 2:
      rainbow();
      break;
    case 3:
      rainbowCycle();
      break;
    case 4:
      off();
      break;
  }
}

void off() {
  setColor(&strip1, strip1.Color(0, 0, 0));
  setColor(&strip2, strip2.Color(0, 0, 0));
}

void standard() {
  setColor(&strip1, DPLColor1);
  setColor(&strip2, DPLColor2);
}

void setColor(Adafruit_NeoPixel* strip, uint32_t color) {
  for(uint16_t i = 0; i < NUM_PIXELS; i++) { 
    strip->setPixelColor(i, color);
  }
  
  strip->show();
}

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip1.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip1.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip1.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void rainbow() {  
  for(int j = 0; j < 256; j++) {
    for(int i = 0; i < strip1.numPixels(); i++) {
      for(int s = 0; s < 2; s++) {
        if(interrupted == 1)
          return 0;
        if(s == 0)
          strip1.setPixelColor(i, Wheel((i + j) & 255));
        else
          strip2.setPixelColor(i, Wheel((i + j) & 255));
      }
    }

    strip1.setBrightness(100);
    strip2.setBrightness(100);
    
    strip1.show();
    strip2.show();
    
    delay(90);
  }
}

void rainbowCycle() {

  for(int j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(int i=0; i< strip1.numPixels(); i++) {
      for(int s = 0; s < 2; s++) {
        if(interrupted == 1) return 0;
        if(s == 0)
          strip1.setPixelColor(i, Wheel(((i * 256 / strip1.numPixels()) + j) & 255));
        else
          strip2.setPixelColor(i, Wheel(((i * 256 / strip2.numPixels()) + j) & 255));
      }
    }

    strip1.setBrightness(100);
    strip2.setBrightness(100);
    
    strip1.show();
    strip2.show();
    
    delay(40);
  }
}

void standardFade() {
  for(int power = -205; power < 205; power++) {
    int brightness = 50 + abs(power);
    
    for(int i = 0; i < strip1.numPixels(); i++) {
        if(interrupted == 1) return 0;
        strip1.setPixelColor(i, DPLColor1);
        strip2.setPixelColor(i, DPLColor2);
        
    }
    strip1.setBrightness(brightness);
    strip2.setBrightness(brightness);
    
    strip1.show();
    strip2.show();
    delay(8);
  }
}
