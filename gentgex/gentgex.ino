#include <TeslaTM1637.h>
#include <microDS3231.h>
#include <TeslaTherm_v2.h>
#include <Tesla_IPS_ST7789.h>
#include <microLED.h>

#define RLED_PIN 6
#define GLED_PIN 3
#define LED_DISP_DIO 7
#define LED_DISP_CLK 8
#define B1_PIN A0
#define B2_PIN A1
#define POT_PIN A3
#define LDR_PIN A6
#define BUZZ_PIN 11
#define TERM_PIN A7
#define IPS_DC 10
#define IPS_RST 4
#define IPS_MOSI 12
#define IPS_SCLK 13
#define TEXT "Завтра у мамы будет День Рождения!"
#define REMIND_DAY 13
#define REMIND_MONTH 9
#define MOTOR 5
#define STRIP_PIN 2
#define NUMLEDS 7
#define HALL_PIN 9


TM1637Display LED_display(LED_DISP_CLK, LED_DISP_DIO);
MicroDS3231 rtc;
IPS_display ips(IPS_DC, IPS_RST, IPS_MOSI, IPS_SCLK);
microLED<NUMLEDS, STRIP_PIN, MLED_NO_CLOCK, LED_WS2812, ORDER_GRB> LEDstrip;

void double_led(int period);
int digits(int num, int row, int col);
void holo_display(int num, int columns = 5);

void setup() {
pinMode(RLED_PIN, OUTPUT);
pinMode(GLED_PIN, OUTPUT);
pinMode(B1_PIN, INPUT_PULLUP);
pinMode(B2_PIN, INPUT_PULLUP);
pinMode(POT_PIN, INPUT);
pinMode(LDR_PIN, INPUT);
pinMode(MOTOR, OUTPUT);
pinMode(HALL_PIN, INPUT_PULLUP);

LED_display.clear();
LED_display.setDots(true);

rtc.begin();
//rtc.setTime(BUILD_SEC, BUILD_MIN, BUILD_HOUR, BUILD_DAY, BUILD_MONTH, BUILD_YEAR);

ips.begin();
ips.setTextSize(3);
ips.setTextColor(RED);
ips.print("START");
delay(500);
ips.fillScreen(BLACK);

ips.setTextColor(CYAN);
ips.setCursor(90, 14);
ips.printRus("градусов");

LED_display.showNumber(rtc.getMonth(), 2, 2);
LED_display.showNumber(rtc.getDate(), 0, 2);
}

int alarm_h = 18;
int alarm_m = 18;
int temp;
int sec, min, hour, day, month;

void loop() {
double_led(2000);

if (sec != rtc.getSeconds()) {
  hour = rtc.getHours();
  min = rtc.getMinutes();
  sec = rtc.getSeconds();
  day = rtc.getDate();
  month = rtc.getMonth();

  if (digitalRead(B1_PIN) == 0) {
  }
  if (digitalRead(B2_PIN) == 0) {
    noTone(BUZZ_PIN);
  }

  int speed = map(analogRead(POT_PIN), 0, 1023, 0, 255);
  analogWrite(MOTOR, speed);
}

if (sec == 0) {
  int brightness = map(analogRead(LDR_PIN), 100, 800, 7, 0);
  LED_display.setBrightness(brightness);

  if (alarm_h == hour and alarm_m == min and sec == 0) {
    tone(BUZZ_PIN, 4000);
  }

  if (temp != getTemp(TERM_PIN)) {
    ips.setTextSize(5);
    ips.setTextColor(BLACK);
    ips.setCursor(15, 0);
    ips.print(temp);
    temp = getTemp(TERM_PIN);
    ips.setTextColor(CYAN);
    ips.setCursor(15, 0);
    ips.print(temp);
  }

  if (hour == 0 and min == 0) {
    LED_display.showNumber(month, 2, 2);
    LED_display.showNumber(day, 0, 2);

    if (day == REMIND_DAY and month == REMIND_MONTH) {
      ips.setCursor(15, 80);
      ips.setTextSize(4);
      ips.setTextColor(PINK);
      ips.printRus("НЕ ЗАБУДЬ");
      ips.setCursor(0, 120);
      ips.setTextColor(YELLOW);
      ips.setTextSize(3);
      ips.printRus(TEXT);
    } else {
      ips.setCursor(15, 80);
      ips.setTextSize(4);
      ips.setTextColor(BLACK);
      ips.printRus("НЕ ЗАБУДЬ");
      ips.setCursor(0, 120);
      ips.setTextColor(BLACK);
      ips.setTextSize(3);
      ips.printRus(TEXT);
    }
  }
}

if (digitalRead(HALL_PIN) == 0) {
  for (int i = 0; i < 30; ++i){
    holo_display(88, 1);
  }
  holo_display(hour / 10);
  holo_display(hour % 10);
  holo_display(42, 3);
  holo_display(min / 10);
  holo_display(min % 10);
  holo_display(42, 3);
  holo_display(sec / 10);
  holo_display(sec % 10);
}
}


void holo_display(int num, int columns = 5) {
for (int y = 0; y < columns; ++y) {
  for (int i = 0; i <= 6; ++i) {
    int check = digits(num, i, y);
    LEDstrip.set(i, mHEX(mMagenta * check));
  }
  LEDstrip.show();
  delay(1);
}
LEDstrip.clear();
LEDstrip.show();
}


int digits(int num, int row, int col) {
if (num == 8) {
  int leds[7][5] = {
    { 0, 1, 1, 1, 0 },
    { 1, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 1 },
    { 0, 1, 1, 1, 0 },
    { 1, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 1 },
    { 0, 1, 1, 1, 0 }
  };
  return leds[row][col];
} else if (num == 1) {
  int leds[7][5] = {
    { 0, 0, 0, 1, 0 },
    { 0, 0, 1, 1, 0 },
    { 0, 1, 0, 1, 0 },
    { 0, 0, 0, 1, 0 },
    { 0, 0, 0, 1, 0 },
    { 0, 0, 0, 1, 0 },
    { 0, 0, 0, 1, 0 }
  };
  return leds[row][col];
} else if (num == 0) {
  int leds[7][5] = {
    { 0, 1, 1, 1, 0 },
    { 1, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 1 },
    { 0, 1, 1, 1, 0 }
  };
  return leds[row][col];
} else if (num == 2) {
  int leds[7][5] = {
    { 0, 1, 1, 1, 0 },
    { 1, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 1 },
    { 0, 0, 0, 1, 0 },
    { 0, 0, 1, 0, 0 },
    { 0, 1, 0, 0, 0 },
    { 1, 1, 1, 1, 1 }
  };
  return leds[row][col];
} else if (num == 3) {
  int leds[7][5] = {
    { 0, 1, 1, 1, 0 },
    { 1, 0, 0, 0, 1 },
    { 0, 0, 0, 0, 1 },
    { 0, 0, 1, 1, 0 },
    { 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 1 },
    { 0, 1, 1, 1, 0 }
  };
  return leds[row][col];
} else if (num == 4) {
  int leds[7][5] = {
    { 0, 0, 0, 1, 0 },
    { 0, 0, 1, 1, 0 },
    { 0, 1, 0, 1, 0 },
    { 1, 0, 0, 1, 0 },
    { 1, 1, 1, 1, 1 },
    { 0, 0, 0, 1, 0 },
    { 0, 0, 0, 1, 0 }
  };
  return leds[row][col];
} else if (num == 5) {
  int leds[7][5] = {
    { 1, 1, 1, 1, 1 },
    { 1, 0, 0, 0, 0 },
    { 1, 0, 0, 0, 0 },
    { 1, 1, 1, 1, 0 },
    { 0, 0, 0, 0, 1 },
    { 0, 0, 0, 0, 1 },
    { 1, 1, 1, 1, 0 }
  };
  return leds[row][col];
} else if (num == 6) {
  int leds[7][5] = {
    { 0, 1, 1, 1, 0 },
    { 1, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0 },
    { 1, 1, 1, 1, 0 },
    { 1, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 1 },
    { 0, 1, 1, 1, 0 }
  };
  return leds[row][col];
} else if (num == 7) {
  int leds[7][5] = {
    { 1, 1, 1, 1, 1 },
    { 1, 0, 0, 0, 1 },
    { 0, 0, 0, 0, 1 },
    { 0, 0, 0, 1, 0 },
    { 0, 0, 1, 0, 0 },
    { 0, 1, 0, 0, 0 },
    { 1, 0, 0, 0, 0 }
  };
  return leds[row][col];
} else if (num == 9) {
  int leds[7][5] = {
    { 0, 1, 1, 1, 0 },
    { 1, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 1 },
    { 0, 1, 1, 1, 1 },
    { 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 1 },
    { 0, 1, 1, 1, 0 }
  };
  return leds[row][col];
} else if (num == 42) {
  int leds[7][3] = {
    { 0, 0, 0 },
    { 0, 1, 0 },
    { 0, 0, 0 },
    { 0, 0, 0 },
    { 0, 0, 0 },
    { 0, 1, 0 },
    { 0, 0, 0 }
  };
  return leds[row][col];
} else if (num == 88) {
  int leds[7][1] = {
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { 0 }
  };
  return leds[row][col];
}
}


void double_led(int period) {
if ((millis() / period) % 2 == 1) {
  digitalWrite(RLED_PIN, LOW);  //один цвет
  digitalWrite(GLED_PIN, HIGH);
} else {
  digitalWrite(RLED_PIN, HIGH);  //другой цвет
  digitalWrite(GLED_PIN, LOW);
}
}

