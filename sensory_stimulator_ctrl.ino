#include "motor.h"
#include "sensor.h"
#include "PIDController.h"
#include "TransferNode.h"
#include "control.h"
#include "config.h"


#include "TimerOne.h"

void setup() {
  // put your setup code here, to run once:

  final int MIN_CURRENT = 0;
  final int MAX_CURRENT = 5;

  sensor torqueEncoder = new sensor();
  sensor loadCell = new sensor();
  sensor dcEncoder = new sensor();
  sensor forceCurrSensor = new sensor();
  sensor velocityCurrSensor = new sensor();

  motor torqueMotor = new motor();
  motor dcMotor = new motor();
  
  PIDController forceController = new PIDController(0, 0, 0, 0);
  PIDController forceToCurrentController = new PIDController(0, 0, 0, 0);
  PIDController velocityController = new PIDController(0, 0, 0, 0);
  PIDController velocityToCurrentController = new PIDController(0, 0, 0, 0);

  TransferNode forceNode = new TransferNode();
  TransferNode velocityNode = new TransferNode();

  forceController.setSetpoint(10);
  forceController.setSensor(loadCell);
  forceController.setTolerance(1);
  forceController.setOutputRange(MIN_CURRENT, MAX_CURRENT);
  forceController.setOutput(forceNode);

  forceToCurrentController.setSetpointSource(forceNode);
  forceToCurrentController.setSensor(forceCurrSensor);
  forceToCurrentController.setTolerance(.2);
  forceToCurrentController.setOutputRange(0, 1);
  forceToCurrentController.setOutput(torqueMotor);

  velocityController.setSetpoint(40);
  velocityController.setSensor(dcEncoder);
  velocityController.setTolerance(.1);
  velocityController.setOutputRange(MIN_CURRENT, MAX_CURRENT);
  velocityController.setOutput(velocityNode);

  velocityToCurrentController.setSetpointSource(velocityNode);
  velocityToCurrentController.setSensor(velocityCurrSensor);
  velocityToCurrentController.setTolerance(.2);
  velocityToCurrentController.setOutputRange(0, 1);
  velocityToCurrentController.setOutput(dcMotor);


  forceToCurrentController.enable();
  forceController.enable();
  velocityToCurrentController.enable();
  velocityController.enable();


}

void loop() {
  // put your main code here, to run repeatedly:

}
