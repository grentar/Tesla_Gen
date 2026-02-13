#include <Adafruit_GFX.h>
#include <Arduino_ST7789.h>
#include <SPI.h>

#define TFT_DC    10
#define TFT_RST   4
#define TFT_MOSI  12
#define TFT_SCLK  13


#define SENSOR A0

Arduino_ST7789 tft = Arduino_ST7789(TFT_DC, TFT_RST, TFT_MOSI, TFT_SCLK);

#define SCREEN_W 240
#define SCREEN_H 240

#define GRAPH_TOP 40
#define GRAPH_HEIGHT 180

int xPos = 0;

void setup() {
  tft.init(240, 240);
  tft.fillScreen(BLACK);

  drawUI();
}

void loop() {

  int value = analogRead(SENSOR);   // 0–1023

  int y = map(value, 0, 1023, GRAPH_HEIGHT, 0);

  drawGraphPoint(y);
  drawValue(value);

  delay(20);
}

void drawUI() {

  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.print("Light sensor");

  tft.drawRect(0, GRAPH_TOP, SCREEN_W, GRAPH_HEIGHT, WHITE);
}

void drawGraphPoint(int yVal) {

  if (xPos >= SCREEN_W) {
    xPos = 0;
    tft.fillRect(1, GRAPH_TOP + 1, SCREEN_W - 2, GRAPH_HEIGHT - 2, BLACK);
  }

  int y = GRAPH_TOP + yVal;

  tft.drawFastVLine(xPos, y, 2, GREEN);

  xPos++;
}

void drawValue(int value) {

  tft.fillRect(120, 10, 100, 20, BLACK);

  tft.setCursor(120, 10);
  tft.setTextColor(YELLOW);
  tft.setTextSize(2);
  tft.print(value);
}
