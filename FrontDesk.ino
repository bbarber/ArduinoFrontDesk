#include <Adafruit_NeoPixel.h>

#define PIN1 2
#define PIN2 3

const int buttonPin = 4;
int lightMode = 0;
int numModes = 3;
bool oldState = HIGH;

#define NUM_PIXELS 120

Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(NUM_PIXELS, PIN1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(NUM_PIXELS, PIN2, NEO_GRB + NEO_KHZ800);

uint32_t DPLColor1 = strip1.Color(0, 50, 0);
uint32_t DPLColor2 = strip2.Color(15, 15, 15);

void setup() {
  Serial.begin(9600);
  
  pinMode(buttonPin, INPUT_PULLUP);
  
  strip1.begin();
  strip2.begin();
  
  strip1.show();
  strip2.show();

  delay(100);
}

void loop() {
  // Get current button state.
  bool newState = digitalRead(buttonPin);

  // Check if state changed from high to low (button press).
  if (newState == LOW && oldState == HIGH) {
    
    // Short delay to debounce button.
    delay(20);
    
    // Check if button is still low after debounce.
    newState = digitalRead(buttonPin);
    
    if (newState == LOW) {
      lightMode++;
      showMode(lightMode % numModes);
    }
  }

  // Set the last button state to the old state.
  oldState = newState;
}

void showMode(int mode) {
  switch(mode) {
    case 0:
      off();
      break;
    case 1:
      standard();
      break;
    case 2:
      off();
      break;
  }
}

void off() {
  setColor(&strip1, strip1.Color(0, 10, 0));
  setColor(&strip2, strip2.Color(10, 0, 0));
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
