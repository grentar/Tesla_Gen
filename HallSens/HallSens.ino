#define HALL_PIN 9

void setup() {
pinMode(HALL_PIN, INPUT_PULLUP);
Serial.begin(9600);

}

void loop() {
Serial.println(digitalRead(HALL_PIN));
}
