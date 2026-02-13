#define FREQ A0
#define LDR_vol A1
#define LDR_vib A2

const int buzzer   = 8;

// --- сглаживание ---
int smooth(int pin) {
  long sum = 0;
  for (int i = 0; i < 5; i++) {
    sum += analogRead(pin);
    delayMicroseconds(500);
  }
  return sum / 5;
}

void setup() {
  Serial.begin(9600);
}

void loop() {
  // считываем освещённость со сглаживанием
  int rawF = smooth(LDR_freq);
  int rawV = smooth(LDR_vol);
  int rawB = smooth(LDR_vib);

  // диапазон частот (220–1800 Гц)
  int baseFreq = map(rawF, 0, 1023, 220, 1800);

  // мягкое вибрато ±70 Гц
  int vibrato = map(rawB, 0, 1023, -70, 70);

  // итоговая частота
  int freq = constrain(baseFreq + vibrato, 120, 3000);

  // громкость — через ширину импульса 0..80%
  int vol = map(rawV, 0, 1023, 0, 80);

  if (vol < 3) {  
    noTone(buzzer);
    return;
  }

  // создаём огибающую громкости
  tone(buzzer, freq);
  delay(vol);          // слышимая часть
  noTone(buzzer);
  delay(80 - vol);     // тишина как регулировка громкости
}

