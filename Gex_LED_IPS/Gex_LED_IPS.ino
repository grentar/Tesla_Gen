#include <Arduino.h>
#include <GyverSegment.h>
#include <microDS3231.h>
#include <EncButton.h>
#include <Tesla_IPS_ST7789.h>
#include <TeslaTherm_v2.h>

#define RLED_PIN 6
#define GLED_PIN 3
#define DIO_PIN 7
#define CLK_PIN 8
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
#define TEXT "Hello World"
#define REMIND_DAY 13
#define REMIND_MONTH 2

MicroDS3231 rtc;
IPS_display ips(IPS_DC, IPS_RST, IPS_MOSI, IPS_SCLK);
Disp1637Colon disp(DIO_PIN, CLK_PIN, 1);
Button btn0(A0);
Button btn1(A1);

String str = "global String";
SegRunner run(&disp);

bool alarmActive = false; // Флаг активного будильника
int alarm = 1;            // Минуты для срабатывания
int temp = 0;
void setup() {
pinMode(RLED_PIN, OUTPUT);
pinMode(GLED_PIN, OUTPUT);
pinMode(POT_PIN, INPUT);
pinMode(LDR_PIN, INPUT);
  pinMode(BUZZ_PIN, OUTPUT); // Обязательно настроить как выход!

disp.clear();
disp.brightness(7);
rtc.begin();
  // rtc.setTime(BUILD_SEC, BUILD_MIN, BUILD_HOUR, BUILD_DAY, BUILD_MONTH, BUILD_YEAR);

ips.begin();
ips.setTextSize(3);
ips.setTextColor(RED);
ips.print("START");
delay(500);
ips.fillScreen(BLACK);

ips.setTextColor(CYAN);
ips.setCursor(90, 14);
ips.printRus("градусов");
}

void loop() {
  btn0.tick();
  btn1.tick();

  // Мигание светодиодами
  if ((millis() / 200) % 2 == 1) {
    digitalWrite(RLED_PIN, LOW);
    digitalWrite(GLED_PIN, HIGH);
  } else {
    digitalWrite(RLED_PIN, HIGH);
    digitalWrite(GLED_PIN, LOW);
  }

  int min = rtc.getMinutes();
  int sec = rtc.getSeconds();
  int hour = rtc.getHours();
  int day = rtc.getDate();
  int month = rtc.getMonth();
  // Автояркость от фоторезистора
  int brightness = map(analogRead(LDR_PIN), 100, 800, 7, 0);
  disp.brightness(constrain(brightness, 0, 7)); // Защита от выхода за пределы

  // Всегда включаем двоеточие
  disp.colon(1);

  // Срабатывание будильника (только один раз в минуту)
  if (!alarmActive && alarm == min && sec == 0) {
    alarmActive = true;
    tone(BUZZ_PIN, 4000);
    run.setText(str);
    run.start(); // Запуск бегущей строки БЕЗ блокировки
  }

  // Обновление бегущей строки ИЛИ вывод времени
  if (alarmActive) {
    run.tick(); // Неблокирующее обновление текста
  } else {
    disp.showClock(min, sec, 0); // Вывод времени, когда будильник выключен
  }

  // Отключение будильника кнопкой
  if (btn1.click() && alarmActive) {
    noTone(BUZZ_PIN);
    alarmActive = false;
    run.stop(); // Остановка прокрутки
    disp.clear(); // Очистка дисплея перед возвратом к времени
  }



if ((millis() / 500) % 2 == 0) {
  disp.showClock(hour,min);
} else {
 disp.showClock(day,month);
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

if (day == REMIND_DAY and month == REMIND_MONTH) {
  ips.setCursor(15, 80);
  ips.setTextSize(4);
  ips.setTextColor(RED);
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