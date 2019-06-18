# include "OneEasyProtocol.h"

SerialPort::SerialPort() {
	serialPortHandle = INVALID_HANDLE_VALUE;
}

SerialPort::~SerialPort()
{
	if (serialPortHandle != INVALID_HANDLE_VALUE)
		CloseHandle(serialPortHandle);
	serialPortHandle = INVALID_HANDLE_VALUE;
}

int SerialPort::open(std::string port, int baudrate)
{
	std::string adress = "\\\\.\\" + port;

	int error = 0;
	DCB dcb;

	memset(&dcb, 0, sizeof(dcb));

	dcb.DCBlength = sizeof(dcb);

	dcb.BaudRate = baudrate;
	dcb.Parity = NOPARITY;
	dcb.StopBits = ONESTOPBIT;
	dcb.ByteSize = 8;
	dcb.fParity = 0;

	serialPortHandle = CreateFileA(adress.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, NULL, NULL);

	if (serialPortHandle != INVALID_HANDLE_VALUE)
	{
		if (!SetCommState(serialPortHandle, &dcb)) error = 2;
	}
	else
	{
		error = 1;

	}

	if (error != 0) {
		close();
	}
	else {
		clear();
	}

	return error;
}

int SerialPort::findRobotPort(int baudrate, std::string & port, char& robotid)
{
	int error = 0;
	DCB dcb;

	memset(&dcb, 0, sizeof(dcb));

	dcb.DCBlength = sizeof(dcb);

	dcb.BaudRate = baudrate;
	dcb.Parity = NOPARITY;
	dcb.StopBits = ONESTOPBIT;
	dcb.ByteSize = 8;
	dcb.fParity = 0;

	for (int i = 0; i < 255; i++)
	{
		std::string portnr = std::to_string(i);
		std::string adress = "\\\\.\\COM" + portnr;

		serialPortHandle = CreateFileA(adress.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, NULL, NULL);

		if (serialPortHandle != INVALID_HANDLE_VALUE)
		{
			if (!SetCommState(serialPortHandle, &dcb)) error = 2;
			else
			{
				char rxchar;
				while (true)
				{
					rxchar = '0';
					clear();
					if (read(&rxchar, 1) != 0)
					{
						if (rxchar != '0')break;
					}
				}
				std::cout << "Robot Port:";
				std::cout << "COM" + portnr + "\n";
				std::cout << "Robot ID:";
				std::cout << rxchar;
				std::cout << "\n";

				port = "COM" + portnr;
				robotid = rxchar;
				clear();
				close();
				break;
			}
		}
		else
		{
			error = 1;

		}

		if (error != 0) {
			close();
		}
		else {
			clear();
		}
	}
	return error;
}

void SerialPort::close(void)
{
	CloseHandle(serialPortHandle);
	serialPortHandle = INVALID_HANDLE_VALUE;
}

int SerialPort::write(std::string txString, int len = 22)
{
	char cstr[23];
	txString.copy(cstr, 23);

	unsigned char txData[22];

	for (int i = 0; i < 22; i++)
	{
		txData[i] = cstr[i];
	}

	unsigned char* buffer = txData;

	unsigned long result = 0;

	if (serialPortHandle != INVALID_HANDLE_VALUE)
		WriteFile(serialPortHandle, buffer, len, &result, NULL);

	return result;
}

int SerialPort::read(char* buffer, int len = 1)
{
	unsigned long rxlen;
	rxlen = 0;
	if (ReadFile(serialPortHandle, buffer, len, &rxlen, NULL))return rxlen;
	return 0;
}

void SerialPort::clear()
{
	PurgeComm(serialPortHandle, PURGE_RXCLEAR | PURGE_TXCLEAR);
}

std::string Converter::valueToString(float value)
{
	if (value > -1000.0 && value < 1000.0)
	{
		float upvalue = value * 10.0F;
		int temp = trunc(upvalue);
		if (temp < 0) temp = temp * -1;
		std::string strint = std::to_string(temp);
		std::string strraw = "";
		std::string strsigned = "";

		if (value > -1.0 and value < 1.0)
		{
			strraw = "000";
			strraw.append(strint);
		}
		else if (value > -10.0 and value < 10.0)
		{
			strraw = "00";
			strraw.append(strint);
		}
		else if (value > -100.0 and value < 100.0)
		{
			strraw = "0";
			strraw.append(strint);
		}
		else if (value > -1000.0 and value < 1000.0)
		{
			strraw = strint;
		}
		else
		{
			std::cout << "Values not valid! Please check you values. Valid numbers are between -999.9 and 999.9";
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
	else std::cout << "Values not valid! Please check you values. Valid numbers are between -999.9 and 999.9";
	return "0000";
}

Pos::Pos()
{
}

std::string Converter::posToString(Pos position)
{
	std::string strX = valueToString(position.x);
	std::string strY = valueToString(position.y);
	std::string strZ = valueToString(position.z);
	return strX + strY + strZ;
}

Move::Move(SerialPort& serialport) :
	serial(serialport),robotid('1'),deviceid('1')
{
}

void Move::setID(char robotid, char deviceid)
{
	this->robotid = robotid;
	this->deviceid = deviceid;
}

void Move::ptp(float positionX, float positionY, float positionZ, float speed)
{
	Pos pos;
	pos.x = positionX;
	pos.y = positionY;
	pos.z = positionZ;
	std::string s = converter.posToString(pos);
	std::string v = converter.valueToString(speed);
	std::string txstring;
	txstring = txstring + robotid + deviceid + "M" + s + v.erase(0, 1);

	int i = 0;
	if (txstring.length() < 22)
	{
		i = txstring.length();
		while (i < 22)
		{
			txstring = txstring + '#';
			i++;
		}
	}
	serial.clear();
	serial.write(txstring);
	std::cout << "One easy protocol: " + txstring + "\n";
	Sleep(50);

	char rxchar;
	while (true)
	{
		rxchar = '0';
		serial.clear();
		if (serial.read(&rxchar, 1) != 0)
		{
			if (rxchar == robotid) break;
		}
	}
	return;
}

Gripper::Gripper(SerialPort& serialport) :
	serial(serialport), robotid('1'), deviceid('1')
{
}

void Gripper::setID(char robotid, char deviceid)
{
	this->robotid = robotid;
	this->deviceid = deviceid;
}

void Gripper::open()
{
	std::string txstring;
	txstring = txstring + robotid + deviceid + Protocol::gripperopen;

	int i = 0;
	if (txstring.length() < 22)
	{
		i = txstring.length();
		while (i < 22)
		{
			txstring = txstring + '#';
			i++;
		}
	}

	serial.clear();
	serial.write(txstring);
	std::cout << "One easy protocol: " + txstring + "\n";
	Sleep(50);

	char rxchar;
	while (true)
	{
		rxchar = '0';
		serial.clear();
		if (serial.read(&rxchar, 1) != 0)
		{
			if (rxchar == robotid) break;
		}
	}
	return;
}

void Gripper::close()
{
	std::string txstring;
	txstring = txstring + robotid + deviceid + Protocol::gripperclose;

	int i = 0;
	if (txstring.length() < 22)
	{
		i = txstring.length();
		while (i < 22)
		{
			txstring = txstring + '#';
			i++;
		}
	}

	serial.clear();
	serial.write(txstring);
	std::cout << "One easy protocol: " + txstring + "\n";
	Sleep(50);

	char rxchar;
	while (true)
	{
		rxchar = '0';
		serial.clear();
		if (serial.read(&rxchar, 1) != 0)
		{
			if (rxchar == robotid) break;
		}
	}
	return;
}

Light::Light(SerialPort& serialport) :
	serial(serialport), robotid('1'), deviceid('1')
{
}

void Light::setID(char robotid, char deviceid)
{
	this->robotid = robotid;
	this->deviceid = deviceid;
}

void Light::on(float intensity)
{
	std::string vi = converter.valueToString(intensity);
	std::string txstring;
	txstring = txstring + robotid + deviceid + Protocol::lighton;

	int i = 0;
	if (txstring.length() < 18)
	{
		i = txstring.length();
		while (i < 18)
		{
			txstring = txstring + '#';
			i++;
		}
	}

	txstring = txstring + vi.erase(0, 1);

	serial.clear();
	serial.write(txstring);
	std::cout << "One easy protocol: " + txstring + "\n";
	Sleep(50);

	char rxchar;
	while (true)
	{
		rxchar = '0';
		serial.clear();
		if (serial.read(&rxchar, 1) != 0)
		{
			if (rxchar == robotid) break;
		}
	}
	return;
}

void Light::off()
{
	std::string txstring;
	txstring = txstring + robotid + deviceid + Protocol::lightoff;

	int i = 0;
	if (txstring.length() < 22)
	{
		i = txstring.length();
		while (i < 22)
		{
			txstring = txstring + '#';
			i++;
		}
	}

	serial.clear();
	serial.write(txstring);
	std::cout << "One easy protocol: " + txstring + "\n";
	Sleep(50);

	char rxchar;
	while (true)
	{
		rxchar = '0';
		serial.clear();
		if (serial.read(&rxchar, 1) != 0)
		{
			if (rxchar == robotid) break;
		}
	}
	return;
}

void Light::setColour(std::string colour, float intensity)
{
	std::string vi = converter.valueToString(intensity);
	std::string txstring;
	txstring = txstring + robotid + deviceid + Protocol::lighton + '#' + colour;

	int i = 0;
	if (txstring.length() < 18)
	{
		i = txstring.length();
		while (i < 18)
		{
			txstring = txstring + '#';
			i++;
		}
	}

	txstring = txstring + vi.erase(0, 1);

	serial.clear();
	serial.write(txstring);
	std::cout << "One easy protocol: " + txstring + "\n";
	Sleep(50);

	char rxchar;
	while (true)
	{
		rxchar = '0';
		serial.clear();
		if (serial.read(&rxchar, 1) != 0)
		{
			if (rxchar == robotid) break;
		}
	}
	return;
}

void Light::setIntensity(float intensity)
{
	std::string vi = converter.valueToString(intensity);
	std::string txstring;
	txstring = txstring + robotid + deviceid + Protocol::lighton;

	int i = 0;
	if (txstring.length() < 18)
	{
		i = txstring.length();
		while (i < 18)
		{
			txstring = txstring + '#';
			i++;
		}
	}

	txstring = txstring + vi.erase(0, 1);

	serial.clear();
	serial.write(txstring);
	std::cout << "One easy protocol: " + txstring + "\n";
	Sleep(50);

	char rxchar;
	while (true)
	{
		rxchar = '0';
		serial.clear();
		if (serial.read(&rxchar, 1) != 0)
		{
			if (rxchar == robotid) break;
		}
	}
	return;
}

ExtMotor::ExtMotor(SerialPort& serialport) :
	serial(serialport), robotid('1'), deviceid('1')
{
}

void ExtMotor::setID(char robotid, char deviceid)
{
	this->robotid = robotid;
	this->deviceid = deviceid;
}

void ExtMotor::stop()
{
	std::string txstring;
	txstring = txstring + robotid + deviceid + Protocol::extmotoroff;

	int i = 0;
	if (txstring.length() < 22)
	{
		i = txstring.length();
		while (i < 22)
		{
			txstring = txstring + '#';
			i++;
		}
	}

	serial.clear();
	serial.write(txstring);
	std::cout << "One easy protocol: " + txstring + "\n";
	Sleep(50);

	char rxchar;
	while (true)
	{
		rxchar = '0';
		serial.clear();
		if (serial.read(&rxchar, 1) != 0)
		{
			if (rxchar == robotid) break;
		}
	}
	return;
}

void ExtMotor::setSpeed(float speed)
{
	std::string v = converter.valueToString(speed);
	std::string txstring;
	txstring = txstring + robotid + deviceid + Protocol::extmotoron;

	int i = 0;
	if (txstring.length() < 18)
	{
		i = txstring.length();
		while (i < 18)
		{
			txstring = txstring + '#';
			i++;
		}
	}

	txstring = txstring + v.erase(0, 1);

	serial.clear();
	serial.write(txstring);
	std::cout << "One easy protocol: " + txstring + "\n";
	Sleep(50);

	char rxchar;
	while (true)
	{
		rxchar = '0';
		serial.clear();
		if (serial.read(&rxchar, 1) != 0)
		{
			if (rxchar == robotid) break;
		}
	}
	return;
}

void ExtMotor::start(float speed)
{
	std::string v = converter.valueToString(speed);
	std::string txstring;
	txstring = txstring + robotid + deviceid + Protocol::extmotoron;

	int i = 0;
	if (txstring.length() < 18)
	{
		i = txstring.length();
		while (i < 18)
		{
			txstring = txstring + '#';
			i++;
		}
	}

	txstring = txstring + v.erase(0, 1);

	serial.clear();
	serial.write(txstring);
	std::cout << "One easy protocol: " + txstring + "\n";
	Sleep(50);

	char rxchar;
	while (true)
	{
		rxchar = '0';
		serial.clear();
		if (serial.read(&rxchar, 1) != 0)
		{
			if (rxchar == robotid) break;
		}
	}
	return;
}

Functions::Functions()
{
}

void Functions::waitFor(int milliseconds)
{
	Sleep(milliseconds);
}

EasyProtocol::EasyProtocol() :
	move(connection),
	gripper(connection),
	light(connection),
	extmotor(connection)
{
}
void EasyProtocol::searchForRobot()
{
	connection.findRobotPort(baudrate, port, robotid);
}
void EasyProtocol::setPort(std::string port, int baudrate)
{
	this->port = port;
	this->baudrate = baudrate;
}
void EasyProtocol::start()
{
	move.setID(this->robotid, this->deviceid);
	gripper.setID(this->robotid, this->deviceid);
	light.setID(this->robotid, this->deviceid);
	extmotor.setID(this->robotid, this->deviceid);
	connection.open(port, baudrate);
}
void EasyProtocol::start(char robotid, char deviceid)
{
	this->robotid = robotid;
	this->deviceid = deviceid;
	move.setID(this->robotid, this->deviceid);
	gripper.setID(this->robotid, this->deviceid);
	light.setID(this->robotid, this->deviceid);
	extmotor.setID(this->robotid, this->deviceid);
	connection.open(port, baudrate);
}
void EasyProtocol::stop()
{
	connection.clear();
	connection.close();
}
;