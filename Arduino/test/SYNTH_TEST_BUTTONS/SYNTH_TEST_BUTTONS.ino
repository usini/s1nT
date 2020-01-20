const int NB_BUTTONS = 12;
const int BUTTON_PINS[NB_BUTTONS] = {4, 5, 6, 7, 8, 9, 10, 12, 2, 3, A2, A3};

void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
  for (int i = 0; i < NB_BUTTONS; i++) {
    pinMode(BUTTON_PINS[i], INPUT_PULLUP);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < NB_BUTTONS; i++) {
    Serial.print(BUTTON_PINS[i]);
    Serial.print("|");
    Serial.print(digitalRead(BUTTON_PINS[i]));
    Serial.print(" ");
  }
  Serial.println("");
}
