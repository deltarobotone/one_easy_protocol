# include "easyprotocol.h"

SerialPort::SerialPort()
{
	serialPortHandle = INVALID_HANDLE_VALUE;
}

SerialPort::~SerialPort()
{
	if (serialPortHandle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(serialPortHandle);
	}
	else
	{
		serialPortHandle = INVALID_HANDLE_VALUE;
	}
}

void SerialPort::open(std::string port, int baudrate)
{
	std::string adress = "\\\\.\\" + port;

	DCB dcb;

	memset(&dcb, 0, sizeof(dcb));
	
	dcb.DCBlength = sizeof(dcb);
	dcb.BaudRate = baudrate;
	dcb.Parity = NOPARITY;
	dcb.StopBits = ONESTOPBIT;
	dcb.ByteSize = 8;
	dcb.fParity = 0;

	serialPortHandle = CreateFileA(adress.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, NULL, NULL);

	COMMTIMEOUTS timeouts;
	timeouts.ReadIntervalTimeout = 1;
	timeouts.ReadTotalTimeoutMultiplier = 1;
	timeouts.ReadTotalTimeoutConstant = 1;
	timeouts.WriteTotalTimeoutMultiplier = 1;
	timeouts.WriteTotalTimeoutConstant = 1;
	SetCommTimeouts(serialPortHandle, &timeouts);

	bool error = false;

	if (serialPortHandle != INVALID_HANDLE_VALUE)
	{
		if (!SetCommState(serialPortHandle, &dcb))
		{
			error = true;
		}
	}
	else
	{
		error = true;
	}

	if (error == true)
	{
		connected = false;
		clear();
	}
	else
	{
		connected = true;
	}
}

void SerialPort::close(void)
{
	CloseHandle(serialPortHandle);
	serialPortHandle = INVALID_HANDLE_VALUE;
	connected = false;
}

int SerialPort::write(std::string txString, int length = 22)
{
	unsigned long txlength = 0;
	if (isOpen())
	{
		char cstr[23];

		strcpy_s(cstr, txString.c_str());

		unsigned char txData[22];

		for (int i = 0; i < 22; i++)
		{
			txData[i] = cstr[i];
		}

		if (serialPortHandle != INVALID_HANDLE_VALUE)
			WriteFile(serialPortHandle, txData, length, &txlength, NULL);
	}
	return txlength;
}

int SerialPort::read(char &buffer, int length = 1)
{
	unsigned long rxlength = 0;
	if (isOpen())
	{
		memset(&buffer, 0, length);
		if (ReadFile(serialPortHandle, &buffer, length, &rxlength, NULL))return rxlength;
	}
	return rxlength;
}

void SerialPort::clear()
{
	PurgeComm(serialPortHandle, PURGE_RXCLEAR | PURGE_TXCLEAR);
}

bool SerialPort::isOpen()
{
	return connected;
}


std::string Utils::valueToString(float value)
{
	if (value > -1000.0 && value < 1000.0)
	{
		float upvalue = value * 10.0F;
		int temp = trunc(upvalue);
		if (temp < 0) temp = temp * -1;
		std::string strint = std::to_string(temp);
		std::string strraw = "";
		std::string strsigned = "";

		if (value > -1.0 && value < 1.0)
		{
			strraw = "000";
			strraw.append(strint);
		}
		else if (value > -10.0 && value < 10.0)
		{
			strraw = "00";
			strraw.append(strint);
		}
		else if (value > -100.0 && value < 100.0)
		{
			strraw = "0";
			strraw.append(strint);
		}
		else if (value > -1000.0 && value < 1000.0)
		{
			strraw = strint;
		}
		else
		{
			if (info == true)std::cout << "Values not valid! Please check your values. Valid numbers are between -999.9 and 999.9";
			return "0000";
		}

		if (value >= 0.0)
		{
			strsigned = "+";
			strsigned.append(strraw);
		}
		else
		{
			strsigned = "-";
			strsigned.append(strraw);
		}
		return strsigned;
	}
	else if (info == true)std::cout << "Values not valid! Please check your values. Valid numbers are between -999.9 and 999.9";
	return "0000";
}

Pos::Pos()
{
}

std::string Utils::posToString(Pos position)
{
	std::string strX = valueToString(position.x);
	std::string strY = valueToString(position.y);
	std::string strZ = valueToString(position.z);
	return strX + strY + strZ;
}

void Utils::fillData(std::string &txString, int length)
{
	int i = 0;
	if (txString.length() < length)
	{
		i = txString.length();
		while (i < length)
		{
			txString = txString + '#';
			i++;
		}
	}
}

void Utils::printInfo(bool status)
{
	info = status;
}

Move::Move(SerialPort & serialport, bool info) :Basic(serialport,info)
{
}

void Move::ptp(float positionX, float positionY, float positionZ, float speed)
{
	if (connected)
	{
		Pos pos;
		pos.x = positionX;
		pos.y = positionY;
		pos.z = positionZ;
		std::string s = utils.posToString(pos);
		std::string v = utils.valueToString(speed);
		std::string txString;
		txString = txString + robotid + deviceid + "M" + s + v.erase(0, 1);
		utils.fillData(txString);
		serial.clear();
		serial.write(txString);
		if (info == true) std::cout << "One easy protocol: " << txString << "\n";
		waitForRobot();
	}
	return;
}

Gripper::Gripper(SerialPort & serialport, bool info) :Basic(serialport, info)
{
}

void Gripper::open()
{
	if (connected)
	{
		std::string txString;
		txString = txString + robotid + deviceid + Protocol::gripperopen;
		utils.fillData(txString);
		serial.clear();
		serial.write(txString);
		if (info == true) std::cout << "One easy protocol: " << txString << "\n";
		waitForRobot();
	}
	return;
}

void Gripper::close()
{
	if (connected)
	{
		std::string txString;
		txString = txString + robotid + deviceid + Protocol::gripperclose;
		utils.fillData(txString);
		serial.clear();
		serial.write(txString);
		if (info == true) std::cout << "One easy protocol: " << txString << "\n";
		waitForRobot();
	}
	return;
}

Light::Light(SerialPort & serialport, bool info) :Basic(serialport, info)
{
}

void Light::on(float intensity)
{
	if (connected)
	{
		std::string vi = utils.valueToString(intensity);
		std::string txString;
		txString = txString + robotid + deviceid + Protocol::lighton;
		utils.fillData(txString, 18);
		txString = txString + vi.erase(0, 1);
		serial.clear();
		serial.write(txString);
		if (info == true) std::cout << "One easy protocol: " << txString << "\n";
		waitForRobot();
	}
	return;
}

void Light::off()
{
	if (connected)
	{
		std::string txString;
		txString = txString + robotid + deviceid + Protocol::lightoff;
		utils.fillData(txString);
		serial.clear();
		serial.write(txString);
		if (info == true) std::cout << "One easy protocol: " << txString << "\n";
		waitForRobot();
	}
	return;
}

void Light::setColour(std::string colour, float intensity)
{
	if (connected)
	{
		std::string vi = utils.valueToString(intensity);
		std::string txString;
		txString = txString + robotid + deviceid + Protocol::lighton + '#' + colour;
		utils.fillData(txString, 18);
		txString = txString + vi.erase(0, 1);
		serial.clear();
		serial.write(txString);
		if (info == true) std::cout << "One easy protocol: " << txString << "\n";
		waitForRobot();
	}
	return;
}

void Light::setIntensity(float intensity)
{
	if (connected)
	{
		std::string vi = utils.valueToString(intensity);
		std::string txString;
		txString = txString + robotid + deviceid + Protocol::lighton;
		utils.fillData(txString, 18);
		txString = txString + vi.erase(0, 1);
		serial.clear();
		serial.write(txString);
		if (info == true) std::cout << "One easy protocol: " << txString << "\n";
		waitForRobot();
	}
	return;
}

ExtMotor::ExtMotor(SerialPort & serialport, bool info) :Basic(serialport, info)
{
}

void ExtMotor::stop()
{
	if (connected)
	{
		std::string txString;
		txString = txString + robotid + deviceid + Protocol::extmotoroff;
		utils.fillData(txString);
		serial.clear();
		serial.write(txString);
		if (info == true) std::cout << "One easy protocol: " << txString << "\n";
		waitForRobot();
	}
	return;
}

void ExtMotor::setSpeed(float speed)
{
	if (connected)
	{
		std::string v = utils.valueToString(speed);
		std::string txString;
		txString = txString + robotid + deviceid + Protocol::extmotoron;
		utils.fillData(txString, 18);
		txString = txString + v.erase(0, 1);
		serial.clear();
		serial.write(txString);
		if (info == true) std::cout << "One easy protocol: " << txString << "\n";
		waitForRobot();
	}
	return;
}

void ExtMotor::start(float speed)
{
	if (connected)
	{
		std::string v = utils.valueToString(speed);
		std::string txString;
		txString = txString + robotid + deviceid + Protocol::extmotoron;
		utils.fillData(txString, 18);
		txString = txString + v.erase(0, 1);
		serial.clear();
		serial.write(txString);
		if (info == true) std::cout << "One easy protocol: " << txString << "\n";
		waitForRobot();
	}
	return;
}

Functions::Functions(SerialPort & serialport, bool info) :Basic(serialport, info)
{
}

void Functions::waitFor(int milliseconds)
{
	if (connected)
	{
		Sleep(milliseconds);
	}
}

EasyProtocol::EasyProtocol(bool info) :
	move(connection, info),
	gripper(connection, info),
	light(connection, info),
	extmotor(connection, info),
	functions(connection, info),
	info(info)
{
}
EasyProtocol::~EasyProtocol()
{
	connection.clear();
	connection.close();
}
void EasyProtocol::findRobot()
{
	findPorts(baudrate, port, robotid);
}
void EasyProtocol::setPort(std::string port, int baudrate)
{
	this->port = port;
	this->baudrate = baudrate;
}
void EasyProtocol::start()
{
	connected = setCommunication();
	move.setSerialStatus(connected);
	gripper.setSerialStatus(connected);
	light.setSerialStatus(connected);
	extmotor.setSerialStatus(connected);
	functions.setSerialStatus(connected);

}
void EasyProtocol::start(char robotid, char deviceid)
{
	this->robotid = robotid;
	this->deviceid = deviceid;
	move.setID(this->robotid, this->deviceid);
	gripper.setID(this->robotid, this->deviceid);
	light.setID(this->robotid, this->deviceid);
	extmotor.setID(this->robotid, this->deviceid);
	functions.setID(this->robotid, this->deviceid);
	connected = setCommunication();
}
void EasyProtocol::stop()
{
	connection.clear();
	connection.close();
	connected = false;
	move.setSerialStatus(connected);
	gripper.setSerialStatus(connected);
	light.setSerialStatus(connected);
	extmotor.setSerialStatus(connected);
	functions.setSerialStatus(connected);
}
void EasyProtocol::printInfo(bool status)
{
	info = status;
	move.printInfo(info);
	gripper.printInfo(info);
	extmotor.printInfo(info);
	light.printInfo(info);
	functions.printInfo(info);
}

bool EasyProtocol::setCommunication()
{
	if (info == true)std::cout << "Try to connect robot..." << "\n";
	if (info == true)std::cout << "ID: " << robotid << " / " << "Port: " << port << " / " << "Baudrate: " << baudrate << "\n";
	
	connection.open(port, baudrate);
	
	if (connection.isOpen())
	{
		for (int i = 0; i < 10; i++)
		{
			if (info == true) std::cout << "...searching for robot "<< i + 1 << '/' << 10 << "\n";
			char rxchar = ' ';
			connection.clear();
			Sleep(500);
			if (connection.read(rxchar, 1) != 0)
			{
				if (rxchar == robotid)
				{
					if (info == true) std::cout << "...connection sucessfully etablished!" << "\n";
					return true;
				}
			}
		}
		if (info == true) std::cout << "...no robot available. Please check your parameters!" << "\n";
		return false;
	}
	else
	{
		if (info == true) std::cout << "...no robot available. Please connect your robot and activate serial communication software!" << "\n";
		if (info == true) std::cout << "Dont't forget to activate the USB Control Mode (Ctrl) using the switch on the circuit board!" << "\n";
		return false;
	}
}

void EasyProtocol::findPorts(int baudrate, std::string &port, char& robotid)
{
	for (int i = 0; i < 256; i++)
	{
		std::string nr = std::to_string(i);
		std::string portnr = "COM" + nr;
		if (info == true) std::cout << "Checking port: " << portnr << "..." << "\n";

		connection.open(portnr, baudrate);

		if (connection.isOpen())
		{
			char rxchar = ' ';
			char check = ' ';

			connection.clear();
			Sleep(500);
			if (connection.read(rxchar, 1) != 0)
			{
				if (rxchar != ' ' && rxchar != check)
				{
					check = rxchar;
					rxchar = ' ';
				}
			}

			connection.clear();
			Sleep(500);
			if (connection.read(rxchar, 1) != 0)
			{
				if (check != ' ' && rxchar == check)
				{
					if (info == true) std::cout << "...found robot with ID: " << rxchar << " on port: " << portnr << "\n";
					robotid = rxchar;
					port = portnr;
				}
			}
			connection.close();
		}
		if (i == 255)
		{
			if (info == true) std::cout << "...no robot available. Please connect your robot and activate serial communication software!" << "\n";
			if (info == true) std::cout << "Dont't forget to activate the USB Control Mode (Ctrl) using the switch on the circuit board!" << "\n";
			connection.close();
		}
	}
	return;
}

Basic::Basic(SerialPort &serialport, bool info) :
	serial(serialport),
	robotid('1'),
	deviceid('1'),
	info(info)
{
}

void Basic::waitForRobot()
{
	char rxData;
	if (serial.isOpen())
	{
		while (true)
		{
			rxData = ' ';
			serial.clear();
			if (serial.read(rxData, 1) != 0)
			{
				if (rxData == '0')
				{
					std::cout << "WARNING: Position out of workingspace!!! ";
					if (info == true) std::cout << "Please adjust the parameters of robot command" << "\n";
				}
				if (rxData == robotid) break;
			}
		}
	}
}

void Basic::printInfo(bool status)
{
	info = status;
	utils.printInfo(status);
}

void Basic::setID(char robotid, char deviceid)
{
	this->robotid = robotid;
	this->deviceid = deviceid;
}

void Basic::setSerialStatus(bool status)
{
	connected = status;
}
