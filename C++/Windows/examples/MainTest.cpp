# include "OneEasyProtocol.h"
int main()
{
	EasyProtocol robot;
	
	//Option 1
	robot.searchForRobot();
	robot.start();

	//Option 2
	//char robotid = '1';
	//char deviceid = '1';
	//int baudrate = 9600;
	//robot.setPort("COM37",baudrate);
	//robot.start(robotid, deviceid);

	//Gripper
	robot.gripper.open();
	robot.functions.waitFor(2000);
	robot.gripper.close();
	robot.functions.waitFor(2000);

	//Move PTP
	robot.move.ptp(0.0, 0.0, 105, 50.0);
	robot.move.ptp(-30.0, 0.0, 105.0, 85.0);
	robot.move.ptp(-30.0, 0.0, 115.00, 20.00);
	robot.move.ptp(-30.0, 0.0, 105.0, 50.0);
	robot.move.ptp(30.0, 0.0, 105.0, 85.0);
	robot.move.ptp(30.0, 0.0, 115.00, 20.00);
	robot.move.ptp(30.0, 0.0, 105.0, 50.0);
	robot.move.ptp(0.0, 0.0, 85.0, 50.0);

	//Light
	robot.light.setColour(Colour::red, 10.0);
	robot.functions.waitFor(1000);
	robot.light.setColour(Colour::red, 25.0);
	robot.functions.waitFor(1000);
	robot.light.setColour(Colour::red, 50.0);
	robot.functions.waitFor(1000);
	robot.light.setColour(Colour::red, 75.0);
	robot.functions.waitFor(1000);
	robot.light.setColour(Colour::red, 100.0);
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

	//External motor
	robot.extmotor.start();
	robot.functions.waitFor(2000);
	robot.extmotor.setSpeed(50.0);
	robot.functions.waitFor(2000);
	robot.extmotor.stop();
}

