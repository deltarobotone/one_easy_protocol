#include <string>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <iostream> 
#include <fstream>
#include <sstream>
#include <Windows.h>

#ifndef EASYPROTOCOL
#define EASYPROTOCOL
class SerialPort
{
private:
	HANDLE serialPortHandle;
	bool connected;
public:
	SerialPort();
	~SerialPort();
	void open(std::string port = "COM1", int baudrate = 9600);
	void close();
	int write(std::string txString, int lenght);
	int read(char &buffer, int length);
	void clear();
	bool isOpen();
};

namespace Protocol
{
	const std::string gripperopen = "GRIPPEROPEN";
	const std::string gripperclose = "GRIPPERCLOSE";
	const std::string lighton = "LIGHTON";
	const std::string lightoff = "LIGHTOFF";
	const std::string lightred = "RED";
	const std::string lightgreen = "GREEN";
	const std::string lightblue = "BLUE";
	const std::string lightyellow = "YELLOW";
	const std::string lightmagenta = "MAGENTA";
	const std::string lightcyan = "CYAN";
	const std::string lightwhite = "WHITE";
	const std::string extmotoron = "EXTMOTORON";
	const std::string extmotoroff = "EXTMOTOROFF";
};

namespace Colour
{
	const std::string red = Protocol::lightred;
	const std::string green = Protocol::lightgreen;
	const std::string blue = Protocol::lightblue;
	const std::string yellow = Protocol::lightyellow;
	const std::string magenta = Protocol::lightmagenta;
	const std::string cyan = Protocol::lightcyan;
	const std::string white = Protocol::lightwhite;
};

namespace ID
{
	const std::string move = "M";
	const std::string light = "L";
	const std::string gripper = "G";
	const std::string extMmotor = "E";
	const std::string waitFor = "T";
};

class Pos
{
public:
	Pos();
	float x = 0.0;
	float y = 0.0;
	float z = 0.0;
};

class Utils
{
private:
	bool info = true;
public:
	std::string valueToString(float value);
	std::string posToString(Pos position);
	void fillData(std::string &txString, int length=22);
	void printInfo(bool status);
};

class Basic
{
protected:
	char robotid ='1';
	char deviceid = '1';
	bool info = false;
	SerialPort& serial;
	Utils utils;
	bool connected = false;
	void waitForRobot();
public:
	Basic(SerialPort& serialport, bool info = false);
	void printInfo(bool status);
	void setID(char robotid = '1', char deviceid = '1');
	void setSerialStatus(bool status);
};

class Move:public Basic
{
public:
	Move(SerialPort& serialport, bool info);
	void ptp(float positionX, float positionY, float positionZ, float speed = 50.0);
};

class Gripper:public Basic
{
public:
	Gripper(SerialPort& serialport, bool info);
	void open();
	void close();
};

class Light:public Basic
{
public:
	Light(SerialPort& serialport, bool info);

	void on(float intensity = 100.0);
	void off();
	void setColour(std::string colour, float intensity = 100.0);
	void setIntensity(float intensity = 100.0);
};

class ExtMotor:public Basic
{
public:
	ExtMotor(SerialPort& serialport, bool info);
	void start(float speed = 255.0);
	void stop();
	void setSpeed(float speed);
};

class Functions :public Basic
{
public:
	Functions(SerialPort& serialport, bool info);
	void waitFor(int milliseconds);
};

class SmartControlData
{
public:
	std::string dataid = "";
	std::string robotid = "";
	std::string deviceid = "";

	std::string colour = "";
	int intensity = 0; //%

	int xPosition = 0; //mm
	int yPosition = 0; //mm
	int zPosition = 0; //mm
	int velocity = 0;  //%
	bool workingSpaceStatus = false;

	bool gripperStatus = false;

	int waitFortime = 0; //ms

	std::string toString();
	std::string toDataString();
	void fromDataString(std::string dataString);
};

class Flowchart
{
private:
	std::vector<std::string> controlDataList;
	std::vector<SmartControlData> controlDataStore;

public:
	Flowchart(Move& move, Gripper& gripper, Light& light, ExtMotor& extmotor, Functions& functions);
	Functions& functions;
	Move& move;
	Gripper& gripper;
	Light& light;
	ExtMotor& extmotor;
	void print();
	void load(std::string path);
	void start();
};

class EasyProtocol
{
private:
	int baudrate = 9600;
	char robotid = '1';
	char deviceid = '1';
	std::string port = "COM1";
	bool connected = false;
	bool info = false;
	bool setCommunication();
	void findPorts(int baudrate, std::string& port, char& robotid);

public:
	EasyProtocol(bool info = true);
	~EasyProtocol();
	SerialPort connection;
	Functions functions;
	Move move;
	Gripper gripper;
	Light light;
	ExtMotor extmotor;
	Flowchart flowchart;
	void findRobot();
	void setPort(std::string port, int baudrate = 9600);
	void start();
	void start(char robotid, char deviceid);
	void stop();
	void printInfo(bool status);
};

#endif