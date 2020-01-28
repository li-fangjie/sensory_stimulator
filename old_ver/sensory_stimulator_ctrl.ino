#include "motor.h"
#include "sensor.h"
#include "transfer_node.h"
#include "control.h"
#include "control_2.h"
#include "config.h"
#include "pid_controller.h"




void setup() {
  // put your setup code here, to run once:

  int MIN_CURRENT = 0;
  int MAX_CURRENT = 5;

  sensor torqueEncoder = sensor();
  sensor loadCell = sensor();
  sensor dcEncoder = sensor();
  sensor forceCurrSensor = sensor();
  sensor velocityCurrSensor = sensor();

  motor torqueMotor = motor();
  motor dcMotor = motor();
  
  pid_controller forceController = pid_controller();
  pid_controller forceToCurrentController = pid_controller();
  pid_controller velocityController =  pid_controller();
  pid_controller velocityToCurrentController =  pid_controller();

  transfer_node forceNode =  transfer_node();
  transfer_node velocityNode =  transfer_node();

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
