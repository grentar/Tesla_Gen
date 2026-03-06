#include <microLED.h>

#define STRIP_PIN 2
#define NUMLEDS 7

microLED <NUMLEDS, STRIP_PIN, MLED_NO_CLOCK, LED_WS2812, ORDER_GRB> LEDstrip;

void setup() {
 
}

void loop() {
for (int i = 0; i <= 6; ++i) {
  LEDstrip.set(i, mHEX(mMagenta));
  LEDstrip.show();
  delay(50);
  LEDstrip.set(i, mHEX(0));
}
for (int i = 5; i >= 1; --i) {
  LEDstrip.set(i, mHEX(mMagenta));
  LEDstrip.show();
  delay(50);
  LEDstrip.set(i, mHEX(0));
}
}
