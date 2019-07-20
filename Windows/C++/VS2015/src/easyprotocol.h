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
	void findRobot();
	void setPort(std::string port, int baudrate = 9600);
	void start();
	void start(char robotid, char deviceid);
	void stop();
	void printInfo(bool status);
};

#endif