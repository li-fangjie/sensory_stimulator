const int output_pin_1 = 5;
const int output_pin_2 = 6;
const int output_pin_pwm = 4;

void setup() {
  // initialize serial communications at 9600 bps:
  pinMode(output_pin_1, OUTPUT);
  pinMode(output_pin_2, OUTPUT);
  // pinMode(output_pin_pwm, OUTPUT);

  // analogWrite(output_pin_pwm, 0);

  analogWrite(output_pin_pwm, 0);
  digitalWrite(output_pin_1, LOW);
  digitalWrite(output_pin_2, LOW);
}

void loop() {
    digitalWrite(output_pin_1, HIGH);
    digitalWrite(output_pin_2, LOW);
    analogWrite(output_pin_pwm, 150);
}
