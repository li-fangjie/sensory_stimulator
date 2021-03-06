/*
 Setup your scale and start the sketch WITHOUT a weight on the scale
 Once readings are displayed place the weight on the scale
 Press +/- or a/z to adjust the calibration_factor until the output readings match the known weight
 Arduino pin 6 -> HX711 CLK
 Arduino pin 5 -> HX711 DOUT
 Arduino pin 5V -> HX711 VCC
 Arduino pin GND -> HX711 GND 
*/

#include "HX711.h"

HX711 scale;

// float calibration_factor = 1450; // this calibration factor is adjusted according to my load cell
float calibration_factor = -457;
float units;
float ounces;
int count = 0;
float gradient = 0;
float prev = 0;
long int cur_t = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("HX711 calibration sketch");
  Serial.println("Remove all weight from scale");
  Serial.println("After readings begin, place known weight on scale");
  Serial.println("Press + or a to increase calibration factor");
  Serial.println("Press - or z to decrease calibration factor");

  scale.begin(A3, A4);
  scale.set_scale();
  scale.tare();  //Reset the scale to 0

  long zero_factor = scale.read_average(200000); //Get a baseline reading
  // Serial.print("Zero factor: "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
  // Serial.println(zero_factor);

}

void loop() {
  scale.set_scale(calibration_factor); //Adjust to this calibration factor

  if(millis() - cur_t > 50){
    Serial.print("Reading: ");
    units = scale.get_units(10);
    // ounces = units * 0.035274;
    Serial.println(units);
    cur_t = millis();
  }
  /*
  if(count == 10){
    gradient = (units-prev)/10.0;
    Serial.println((units-prev)/10, 10);
    prev = units;
    count = 0;
  }
  /*
  ++count;
  /*
  Serial.print(" g"); 
  Serial.print(" calibration_factor: ");
  Serial.print(calibration_factor);
  Serial.println();
  */
  // delay(500);

  if(Serial.available())
  {
    char temp = Serial.read();
    if(temp == '+' || temp == 'a')
      calibration_factor += 1;
    else if(temp == '-' || temp == 'z')
      calibration_factor -= 1;
    else if(temp == 'm')
      cur_t = millis();
  }
}
