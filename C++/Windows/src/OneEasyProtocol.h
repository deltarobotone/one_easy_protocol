#include <iostream>
#include <string>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <Windows.h>

#ifndef EASYPROTOCOL
#define EASYPROTOCOL
class SerialPort
{
private:
	HANDLE serialPortHandle;

public:
	SerialPort();
	~SerialPort();

	int findRobotPort(int baudrate, std::string& port, char& robotid);

	int open(std::string port, int baudrate);
	void close();

	int write(std::string txString, int len);
	int read(char* buffer, int len);

	void clear();
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


class Pos
{
public:
	Pos();
	float x = 0.0;
	float y = 0.0;
	float z = 0.0;
};

class Converter
{
public:
	std::string valueToString(float value);
	std::string posToString(Pos position);
};

class Move
{
private:
	char robotid;
	char deviceid;
	SerialPort& serial;
	Converter converter;
public:
	Move(SerialPort& serialport);
	void setID(char robotid = '1', char deviceid = '1');
	void ptp(float positionX, float positionY, float positionZ, float speed = 50.0);
};

class Gripper
{
private:
	char robotid;
	char deviceid;
	SerialPort& serial;
public:
	Gripper(SerialPort& serialport);
	void setID(char robotid = '1', char deviceid = '1');
	void open();
	void close();
};

class Light
{
private:
	char robotid;
	char deviceid;
	SerialPort& serial;
	Converter converter;
public:
	Light(SerialPort& serialport);
	void setID(char robotid = '1', char deviceid = '1');
	void on(float intensity = 100.0);
	void off();
	void setColour(std::string colour, float intensity = 100.0);
	void setIntensity(float intensity = 100.0);
};

class ExtMotor
{
private:
	char robotid;
	char deviceid;
	SerialPort& serial;
	Converter converter;
public:
	ExtMotor(SerialPort& serialport);
	void setID(char robotid = '1', char deviceid = '1');
	void start(float speed = 100.0);
	void stop();
	void setSpeed(float speed);
};

class Functions
{

public:
	Functions();
	void waitFor(int milliseconds);
};

class EasyProtocol
{
private:
	std::string serialport = "";
	int baudrate = 9600;
	char robotid = '1';
	char deviceid = '1';
	std::string port = "COM1";
public:
	EasyProtocol();
	SerialPort connection;
	Functions functions;
	Move move;
	Gripper gripper;
	Light light;
	ExtMotor extmotor;
	void searchForRobot();
	void setPort(std::string port, int baudrate = 9600);
	void start();
	void start(char robotid, char deviceid);
	void stop();
};

#endif