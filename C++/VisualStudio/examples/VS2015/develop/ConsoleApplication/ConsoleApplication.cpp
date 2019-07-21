#include "stdafx.h"
#include "easyprotocol.h"

int main()
{
	//Option 1: Automatic with console info enabled
	EasyProtocol robot(true);

	//Find robot port an ID automatically
	robot.findRobot();

	//Start communication
	robot.start();

	//Option 2: Manual with console info enabled
	//EasyProtocol robot(true);

	//Set port, baudrate and timeout of serial communication
	//char robotid = '1';
	//char deviceid = '1';
	//int baudrate = 9600;
	//robot.setPort("COM37",baudrate);

	//Start communication and set ID's
	//robot.start(robotid, deviceid);

	//Robot control functions

	//Gripper
	robot.gripper.open();
	robot.functions.waitFor(2000);
	robot.gripper.close();
	robot.functions.waitFor(2000);

	//Move(pick and place)
	//ptp(X - Position, Y - Position, Z - Position, Speed)
	//Note: Only one number after the comma is used by the protocol and the robot
	robot.move.ptp(0.0, 0.0, 85.0, 50.0);
	robot.move.ptp(-30.0, 0.0, 105.0, 85.0);
	robot.move.ptp(-30.0, 0.0, 115.0, 20.0);
	robot.move.ptp(-30.0, 0.0, 105.0, 50.0);
	robot.move.ptp(30.0, 0.0, 105.0, 85.0);
	robot.move.ptp(30.0, 0.0, 115.0, 20.0);
	robot.move.ptp(30.0, 0.0, 105.0, 50.0);
	robot.move.ptp(0.0, 0.0, 105, 5.0);

	//Disable console info
	robot.printInfo(false);
	
	//Light intensity
	robot.light.setColour(Colour::red, 10.0);
	robot.functions.waitFor(1000);
	robot.light.setColour(Colour::red, 50.0);
	robot.functions.waitFor(1000);
	robot.light.setColour(Colour::red, 100.0);

	//Light colours
	robot.functions.waitFor(1000);
	robot.light.setColour(Colour::blue);
	robot.functions.waitFor(1000);
	robot.light.setColour(Colour::green);
	robot.functions.waitFor(1000);
	robot.light.setColour(Colour::yellow);
	robot.functions.waitFor(1000);
	robot.light.setColour(Colour::magenta);
	robot.functions.waitFor(1000);
	robot.light.setColour(Colour::cyan);
	robot.functions.waitFor(1000);
	robot.light.setColour(Colour::white);
	robot.functions.waitFor(1000);

	//Light off
	robot.light.off();
	robot.functions.waitFor(1000);

	//External motor control
	robot.extmotor.start();
	robot.functions.waitFor(2000);
	robot.extmotor.setSpeed(50.0);
	robot.functions.waitFor(2000);
	robot.extmotor.stop();

	//Stop communication
	robot.stop();
    return 0;
}

