/*  SparkFun ACS712 and ACS723 Demo
    Created by George Beckstein for SparkFun
    4/30/2017
    Updated by SFE
    6/14/2018

    Uses an Arduino to set up the ACS712 and ACS723 Current Sensors
    See the tutorial at: https://learn.sparkfun.com/tutorials/current-sensor-breakout-acs723-hookup-guide

    Parts you may need:
    - 100 Ohm, 1/2W or greater resistor OR two 220 Ohm 1/4 resistors in parallel
    - ACS712 Breakout with on-board amplifier or ACS723 Current Sensor (Low Current)

    Optional equipment:
    - Oscilloscope
    - Multimeter (or two)
    - A power supply with current limiting/constant current would be handy to calibrate the device without using resistors
*/

const int analogInPin = A2;
const int output_pin_pwm_b = 4;
const int output_pin_1b = 5;
const int output_pin_2b = 6;
const int output_pin_1 = 12;
const int output_pin_2 = 7;
const int output_pin_pwm = 13;

// Number of samples to average the reading over
// Change this to make the reading smoother... but beware of buffer overflows!
const int avgSamples = 1;

int sensorValue = 0;

// float sensitivity = 100.0 / 500.0; //100mA per 500mV = 0.2
// float sensitivity = 1.0705;
float sensitivity = 0.59426;
float Vref = 2497; // Output voltage with no current: ~ 2500mV or 2.5V
float curr_offset = 38;
long int sum_v = 0;
int num = 20;
int cur_avg = 0;

int cur_in = 0;

float calibrate(){
  float sensor_sum = 0;
  float out = 0;
  for(int i=0; i<2000; i++){
    sensor_sum += analogRead(analogInPin);
    delay(2);
  }
  out = sensor_sum * 4.88 / 2000;
  return out;
}

float get_offset(){
  Serial.println("Please enter measured current, in mA");
  while(Serial.available() <= 0) continue;
  if(Serial.available() > 0) return (int)Serial.parseInt();
  return 0;
}

float get_gain(float v_ref){
  int cur_curr = 0;
  float output = 0;

  Serial.println("Should gain be reset? If so, please ensure that there is a load is in the circuit. y/n");
  while(!Serial.available()) continue;
  if(Serial.available() && Serial.read() == 'n') return 0;
  
  Serial.println("Please enter measured current, in mA");
  while(Serial.available() <= 0) continue;
  if(Serial.available() > 0) cur_curr = (int)Serial.parseInt();

  float sensor_sum = 0;
  float cur_v = 0;
  for(int i=0; i<2000; i++){
    sensor_sum += analogRead(analogInPin);
    delay(2);
  }
  Serial.print("sensor_sum: ");
  Serial.println(sensor_sum);
  cur_v = sensor_sum * 4.88 / 2000.0;
  output = (cur_curr - curr_offset) / (cur_v - v_ref);
  Serial.print("The sensitivity/gain is set to be:");
  Serial.println(output, 5);
  return output;
}

void setup() {
  // initialize serial communications at 9600 bps:
  pinMode(output_pin_1, OUTPUT);
  pinMode(output_pin_2, OUTPUT);
  pinMode(output_pin_1b, OUTPUT);
  pinMode(output_pin_2b, OUTPUT);

  analogWrite(output_pin_pwm, 0);
  digitalWrite(output_pin_1, HIGH);
  digitalWrite(output_pin_2, LOW);

  analogWrite(output_pin_pwm_b, 0);
  digitalWrite(output_pin_1b, HIGH);
  digitalWrite(output_pin_2b, LOW);
  Serial.begin(9600);
  delay(100);
  Vref = calibrate();
  // sensitivity = get_gain(Vref);
  Serial.print("Vref:");
  Serial.println(Vref);
}

void loop() {
    analogWrite(output_pin_pwm, 200);
    // digitalWrite(output_pin_1, HIGH);
    // digitalWrite(output_pin_2, LOW);
    if(Serial.available() > 0){
      cur_in = (int) Serial.parseInt();
      if(cur_in >= 0) analogWrite(output_pin_pwm, cur_in);
      else if (cur_in == -1){
        sensitivity = get_gain(Vref);
      }
      else if(cur_in == -2){
        curr_offset = get_offset();
      }
    }
// read the analog in value:
  for (int i = 0; i < avgSamples; i++)
  {
    sensorValue += analogRead(analogInPin);

    // wait 2 milliseconds before the next loop
    // for the analog-to-digital converter to settle
    // after the last reading:
    delay(2);

  }

  sensorValue = sensorValue / avgSamples;

  // The on-board ADC is 10-bits -> 2^10 = 1024 -> 5V / 1024 ~= 4.88mV
  // The voltage is in millivolts
  float voltage = 4.88 * sensorValue;

  // This will calculate the actual current (in mA)
  // Using the Vref and sensitivity settings you configure
  float current = (voltage - Vref) * sensitivity;

  // This is the raw sensor value, not very useful without some calculations
  //Serial.print(sensorValue);

  /*************************************************************************************
   * Step 1.)
   * Uncomment and run the following code to set up the baseline voltage 
   * (the voltage with 0 current flowing through the device).
   * Make sure no current is flowing through the IP+ and IP- terminals during this part!
   * 
   * The output units are in millivolts. Use the Arduino IDE's Tools->Serial Plotter
   * To see a plot of the output. Adjust the Vref potentiometer to set the reference
   * voltage. This allows the sensor to output positive and negative currents!
   *************************************************************************************/

  // Serial.print(voltage);
  //Serial.print("mV");

  /*************************************************************************************
   * Step 2.)
   * Keep running the same code as above to set up the sensitivity
   * (how many millivolts are output per Amp of current.
   * 
   * This time, use a known load current (measure this with a multimeter)
   * to give a constant output voltage. Adjust the sensitivity by turning the
   * gain potentiometer.
   * 
   * The sensitivity will be (known current)/(Vreading - Vref).
   *************************************************************************************/

    /*************************************************************************************
   * Step 3.)
   * Comment out the code used for the last two parts and uncomment the following code.
   * When you have performed the calibration steps above, make sure to change the 
   * global variables "sensitivity" and "Vref" to what you have set up.
   * 
   * This next line of code will print out the calculated current from these parameters.
   * The output is in mA
   *************************************************************************************/

  sum_v = sum_v + current - cur_avg;
  cur_avg = sum_v / num;
  Serial.print((cur_avg + 38.0));
  //Serial.print("mA");


  // -- DO NOT UNCOMMENT BELOW THIS LINE --
  Serial.print("\n");

  // Reset the sensor value for the next reading
  sensorValue = 0;
}


