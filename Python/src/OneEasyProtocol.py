
# coding: utf-8

# In[7]:


import serial
import time
import sys

class Pos(object):
    def __init__(self):
        self.x = 0.0
        self.y = 0.0
        self.z = 0.0

class ServoData(object):
    def __init__(self):
        self.a = 0.0
        self.b = 0.0
        self.c = 0.0
        
class Protocol(object):
    def __init__(self):
        self.gripperopen =  'GRIPPEROPEN'
        self.gripperclose = 'GRIPPERCLOSE'
        self.lighton =      'LIGHTON'
        self.lightoff =     'LIGHTOFF'
        self.lightred =     'RED'
        self.lightgreen =   'GREEN'
        self.lightblue =    'BLUE'
        self.lightyellow =  'YELLOW'
        self.lightmagenta = 'MAGENTA'
        self.lightcyan =    'CYAN'
        self.lightwhite =   'WHITE'
        self.extmotoron =   'EXTMOTORON'
        self.extmotoroff =  'EXTMOTOROFF'
        
class Color(object):
    def __init__(self):
        self.__protocol = Protocol()
        self.red =     self.__protocol.lightred
        self.green =   self.__protocol.lightgreen
        self.blue =    self.__protocol.lightblue
        self.yellow =  self.__protocol.lightyellow
        self.magenta = self.__protocol.lightmagenta
        self.cyan =    self.__protocol.lightcyan
        self.white =   self.__protocol.lightwhite
        
class Converter(object):
    def __init__(self):
        return None
    
    def valueToString(self,value):
        
        if(value>-1000.0 and value<1000.0):
            
            upvalue = value * 10
            temp = int(upvalue)
            string = str(temp)
            
            if (value > -1.0 and value < 1.0):
                string = '000' + string
                
            elif(value> -10.0 and value < 10.0):
                string = '00' + string
                
            elif(value> -100.0 and value < 100.0):
                string = '0' + string
            
            elif (value > -1000.0 and value < 1000.0):
                string = string 
            else:
                return '0000'
                
            if(value>=0.0):
                string = '+' + string
            else:
                string = string.replace('-','')
                string = '-' + string
            return string
        
        else:
            print('Values not valid! Please check you values. Valid numbers are between -999.9 and 999.9')
            return '0000'
        
    def posToString(self,pos):
        strX=self.valueToString(pos.x)
        strY=self.valueToString(pos.y)
        strZ=self.valueToString(pos.z)
        return strX,strY,strZ
        
class Functions(object):        
    def __init__(self):
        return None
    
    def waitFor(self,milliseconds):
        seconds = milliseconds/1000
        time.sleep(int(seconds))
        return None
    
class Move(object):
    def __init__(self, connection=None, robotid=None, deviceid=None):
        self.__robotid = robotid
        self.__deviceid = deviceid
        self.__connection = connection
        self.__protocol = Protocol()
        self.__converter = Converter()
        self.__pos = Pos()
         
    def __checkParameters(self):
        if( self.__robotid==None and self.__deviceid==None and self.__connection==None):
            print("...no connection available. Please connect a robot sucessfully with start()-function!")
            return 1
        else:
            return 0
        
    def ptp(self,positionX,positionY,positionZ,speed=50.0):
        if(self.__checkParameters()==0):
            self.__pos.x = positionX
            self.__pos.y = positionY
            self.__pos.z = positionZ
            x,y,z = self.__converter.posToString(self.__pos)
            v = self.__converter.valueToString(speed)
            v = v.replace('+','')
            v = v.replace('-','')
            txstring = self.__robotid + self.__deviceid +'M'+ x + y + z + v
            i=0
            if len(txstring) < 22:
                i=len(txstring)
                while(i<22):
                    txstring = txstring +'#' 
                    i+=1
            txbytes = str.encode(txstring)
            self.__connection.reset_output_buffer()
            time.sleep(0.01)
            self.__connection.write(txbytes)
            print('One easy protocol: ' + txstring)
            self.__connection.reset_input_buffer()
            time.sleep(0.01)
            while True:
                line = self.__connection.read(1)
                if line == self.__robotid.encode():
                    break
        return 0
        
    def home(self):
        if(self.__checkParameters()==0):
            home = "+0000+0000+0850+0500"
            txstring = self.__robotid + self.__deviceid + home
            if len(txstring) < 22:
                i=len(txstring)
                while(i<22):
                    txstring = txstring +'#' 
                    i+=1
            txbytes = str.encode(txstring)
            self.__connection.reset_output_buffer()
            time.sleep(0.01)
            self.__connection.write(txbytes)
            print('One easy protocol: ' + txstring)
            self.__connection.reset_input_buffer()
            time.sleep(0.01)
            while True:
                line = self.__connection.read(1)
                if line == self.__robotid.encode():
                    break
        return 0
        
#     def getPosition(self):
#         if(self.__checkParameters()==0):
#         return 0

#     def getAngles(self):
#         if(self.__checkParameters()==0):
#         return 0
    
class Gripper(object):
    def __init__(self, connection=None, robotid=None, deviceid=None):
        self.__robotid = robotid
        self.__deviceid = deviceid
        self.__connection = connection
        self.__protocol = Protocol()
        
    def __checkParameters(self):
        if( self.__robotid==None and self.__connection==None):
            print("...no connection available. Please connect a robot sucessfully with start()-function!")
            return 1
        else:
            return 0
    
    def open(self):
        if(self.__checkParameters()==0):
            txstring = self.__robotid + self.__deviceid + self.__protocol.gripperopen
            if len(txstring) < 22:
                i=len(txstring)
                while(i<22):
                    txstring = txstring +'#' 
                    i+=1
            txbytes = str.encode(txstring)
            self.__connection.reset_output_buffer()
            time.sleep(0.01)
            self.__connection.write(txbytes)
            print('One easy protocol: ' + txstring)
            self.__connection.reset_input_buffer()
            time.sleep(0.01)
            while True:
                line = self.__connection.read(1)
                if line == self.__robotid.encode():
                    break
        return 0
    
    def close(self):
        if(self.__checkParameters()==0):
            txstring = self.__robotid + self.__deviceid + self.__protocol.gripperclose
            if len(txstring) < 22:
                i=len(txstring)
                while(i<22):
                    txstring = txstring +'#' 
                    i+=1
            txbytes = str.encode(txstring)
            self.__connection.reset_output_buffer()
            time.sleep(0.01)
            self.__connection.write(txbytes)
            print('One easy protocol: ' + txstring)
            self.__connection.reset_input_buffer()
            time.sleep(0.01)
            while True:
                line = self.__connection.read(1)
                if line == self.__robotid.encode():
                    break
        return 0
    
#     def getStatus(self):
#         if(self.__checkParameters()==0):
#             return 0
        
class ExtMotor(object):
    def __init__(self, connection=None, robotid=None, deviceid=None):
        self.__robotid = robotid
        self.__deviceid = deviceid
        self.__connection = connection
        self.__protocol = Protocol()
        self.__converter = Converter()
        
    def __checkParameters(self):
        if( self.__robotid==None and self.__connection==None):
            print("...no connection available. Please connect a robot sucessfully with start()-function!")
            return 1
        else:
            return 0
        
    
    def start(self,speed=100.0):
        if(self.__checkParameters()==0):
            v = self.__converter.valueToString(speed)
            v = v.replace('+','')
            v = v.replace('-','')
            txstring = self.__robotid + self.__deviceid + self.__protocol.extmotoron + v
            if len(txstring) < 18:
                i=len(txstring)
                while(i<18):
                    txstring = txstring +'#' 
                    i+=1
                v = self.__converter.valueToString(speed)
                v = v.replace('+','')
                v = v.replace('-','')
                txstring = txstring + v
            txbytes = str.encode(txstring)
            self.__connection.reset_output_buffer()
            time.sleep(0.01)
            self.__connection.write(txbytes)
            print('One easy protocol: ' + txstring)
            self.__connection.reset_input_buffer()
            time.sleep(0.01)
            while True:
                line = self.__connection.read(1)
                if line == self.__robotid.encode():
                    break
        return 0
        
    def stop(self):
        if(self.__checkParameters()==0):
            txstring = self.__robotid + self.__deviceid + self.__protocol.extmotoroff
            if len(txstring) < 22:
                i=len(txstring)
                while(i<22):
                    txstring = txstring +'#' 
                    i+=1
            txbytes = str.encode(txstring)
            self.__connection.reset_output_buffer()
            time.sleep(0.01)
            self.__connection.write(txbytes)
            print('One easy protocol: ' + txstring)
            self.__connection.reset_input_buffer()
            time.sleep(0.01)
            while True:
                line = self.__connection.read(1)
                if line == self.__robotid.encode():
                    break
        return 0
        
    def setSpeed(self,speed):
        if(self.__checkParameters()==0):
            v = self.__converter.valueToString(speed)
            v = v.replace('+','')
            v = v.replace('-','')
            txstring = self.__robotid + self.__deviceid + self.__protocol.extmotoron
            if len(txstring) < 18:
                i=len(txstring)
                while(i<18):
                    txstring = txstring +'#' 
                    i+=1
                v = self.__converter.valueToString(speed)
                v = v.replace('+','')
                v = v.replace('-','')
                txstring = txstring + v
            txbytes = str.encode(txstring)
            self.__connection.reset_output_buffer()
            time.sleep(0.01)
            self.__connection.write(txbytes)
            print('One easy protocol: ' + txstring)
            self.__connection.reset_input_buffer()
            time.sleep(0.01)
            while True:
                line = self.__connection.read(1)
                if line == self.__robotid.encode():
                    break
        return 0
        
#     def getSpeed(self):
#         if(self.__checkParameters()==0):
#         return 0

#     def getStatus(self):
#         if(self.__checkParameters()==0):
#         return 0
        
class Light(object):
    def __init__(self, connection=None, robotid=None, deviceid=None):
        self.__robotid = robotid
        self.__deviceid = deviceid
        self.__connection = connection
        self.__protocol = Protocol()
        self.__converter = Converter()
        
    def __checkParameters(self):
        if( self.__robotid==None and self.__connection==None):
            print("...no connection available. Please connect a robot sucessfully with start-function!")
            return 1
        else:
            return 0
    
    def on(self, intensity=100.0):
        if(self.__checkParameters()==0):
            txstring = self.__robotid + self.__deviceid + self.__protocol.lighton
            if len(txstring) < 18:
                i=len(txstring)
                while(i<18):
                    txstring = txstring +'#' 
                    i+=1
                ledi = self.__converter.valueToString(intensity)
                ledi = ledi.replace('+','')
                ledi = ledi.replace('-','')
                txstring = txstring + ledi
            txbytes = str.encode(txstring)
            self.__connection.reset_output_buffer()
            time.sleep(0.01)
            self.__connection.write(txbytes)
            print('One easy protocol: ' + txstring)
            self.__connection.reset_input_buffer()
            time.sleep(0.01)
            while True:
                line = self.__connection.read(1)
                if line == self.__robotid.encode():
                    break
        return 0
        
    def off(self):
        if(self.__checkParameters()==0):
            txstring = self.__robotid + self.__deviceid + self.__protocol.lightoff
            if len(txstring) < 22:
                i=len(txstring)
                while(i<22):
                    txstring = txstring +'#' 
                    i+=1
            txbytes = str.encode(txstring)
            self.__connection.reset_output_buffer()
            time.sleep(0.01)
            self.__connection.write(txbytes)
            print('One easy protocol: ' + txstring)
            self.__connection.reset_input_buffer()
            time.sleep(0.01)
            while True:
                line = self.__connection.read(1)
                if line == self.__robotid.encode():
                    break
        return 0
        
    def setColour(self, colour, intensity = 100.0):
        if(self.__checkParameters()==0):
            txstring = self.__robotid + self.__deviceid + self.__protocol.lighton + '#' + colour
            if len(txstring) < 18:
                i=len(txstring)
                while(i<18):
                    txstring = txstring +'#' 
                    i+=1
                ledi = self.__converter.valueToString(intensity)
                ledi = ledi.replace('+','')
                ledi = ledi.replace('-','')
                txstring = txstring + ledi
            txbytes = str.encode(txstring)
            self.__connection.reset_output_buffer()
            time.sleep(0.01)
            self.__connection.write(txbytes)
            print('One easy protocol: ' + txstring)
            self.__connection.reset_input_buffer()
            time.sleep(0.01)
            while True:
                line = self.__connection.read(1)
                if line == self.__robotid.encode():
                    break
        return 0
    def setIntensity(self, intensity):
        if(self.__checkParameters()==0):
            txstring = self.__robotid + self.__deviceid + self.__protocol.lighton
            if len(txstring) < 18:
                i=len(txstring)
                while(i<18):
                    txstring = txstring +'#' 
                    i+=1
                ledi = self.__converter.valueToString(intensity)
                ledi = v.replace('+','')
                ledi = v.replace('-','')
                txstring = txstring + ledi
            txbytes = str.encode(txstring)
            self.__connection.reset_output_buffer()
            time.sleep(0.01)
            self.__connection.write(txbytes)
            print('One easy protocol: ' + txstring)
            self.__connection.reset_input_buffer()
            time.sleep(0.01)
            while True:
                line = self.__connection.read(1)
                if line == self.__robotid.encode():
                    break
        return 0
             
class EasyProtocol(object):
    def __init__(self):
        self.__timeout = 1
        self.__port = ""
        self.__baudrate = 9600
        self.__connection = serial.Serial()
        self.__robotid = ''
        
        self.move = Move()
        self.gripper = Gripper()
        self.extmotor = ExtMotor()
        self.light = Light()
        self.functions = Functions()
    
    def setPort(self,port,baudrate = 9600,timeout = 1):
        self.__timeout = timeout
        self.__port = port
        self.__baudrate = baudrate
        return 0
    
    def __setCommunication(self):
        print("Try to connect robot...")
        print("ID: "+ self.__robotid+" / "+"Port: "+self.__port+" / "+"Baudrate: "+str(self.__baudrate))
        traffic=None
        try:
            self.__connection = serial.Serial(port=self.__port,baudrate=self.__baudrate,timeout=self.__timeout)
            time.sleep(0.25)
            i=0
            z=10
            while i<z:
                print("...searching for robot ("+str(i+1)+'/'+str(z)+')')
                traffic = self.__connection.read(1)
                time.sleep(0.25)
                if traffic == self.__robotid.encode():
                    print("...connection sucessfully etabished!")
                    break
                i+=1 
                traffic = None
        except:
            print("...no robot available. Please check your parameters!")
            
        if traffic == None:
            print("...no robot available. Please connect your robot and activate serial communication software!")
            print("Dont't forget to activate the USB Control Mode (Ctrl) using the switch on the circuit board!")
        elif traffic!=None:
            print("Found robot with ID: " + str(traffic.decode()))
            self.move = Move(self.__connection,self.__robotid,self.__deviceid)
            self.gripper = Gripper(self.__connection,self.__robotid,self.__deviceid)
            self.extmotor = ExtMotor(self.__connection,self.__robotid,self.__deviceid)
            self.light = Light(self.__connection,self.__robotid,self.__deviceid)
        return 0
    
    def __find_ports(self):
        if sys.platform.startswith('win'):
            ports = ['COM%s' % (i+1) for i in range(256)]
        elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
            ports = glob.glob('/dev/ttyUSB*')
        elif sys.platform.startswith('darwin'):
            ports = glob.glob('/dev/tty.usbserial*')
        else:
            print("Can' finding ports on your operating system")
            ports = ""
        return ports
    
    def find_robot(self):
        ports = self.__find_ports()
        if ports != "":
            traffic=None
            portname=""
            for port in ports:
                try:
                    self.__connection = serial.Serial(port,baudrate=self.__baudrate,timeout=self.__timeout)
                    print("Checking port: " + port +"...")
                    time.sleep(0.25)
                    self.__connection.reset_input_buffer()
                    traffic = self.__connection.read(1)
                    time.sleep(0.25)
                    if traffic != None:
                        print("...found robot with ID: " + str(traffic.decode()) + " on port: "+ port)
                        self.__connection.close()
                        self.__robotid = traffic.decode()
                        self.__port = port
                        break
                    self.__robotid = None
                    self.__connection.close()
                except:
                    print("...error while checking port: "+ port) 
            else:
                print("...no robot available. Please connect your robot and activate serial communication software!")
                print("Dont't forget to activate the USB Control Mode (Ctrl) using the switch on the circuit board!")
        return 
    
    def start(self,robotid=1,deviceid=0):
        robotidstr = str(robotid)
        deviceidstr = str(deviceid)
        if len(robotidstr)==1 and len(deviceidstr)==1:
            self.__robotid = robotidstr
            self.__deviceid = deviceidstr
            self.__setCommunication()
        else:
            print("Please enter only one symbol to set the Robot-ID and Device-ID") 
        return 0
    
    def stop(self):
        self.__connection.close()
        
    def __del__(self):
        self.stop()
        
    def __waitForRobot(self):
        print("Robot executes a command...")
        i=0
        z=10
        while i<z:
            line = self.__connection.read(1)
            if line == self.__robotid.encode():
                break
            print("...")
            i+=1
        if i==10:
            print("Timout! Please check the connection to the robot!")
        else:   
            print("Robot is waiting for signals...")
        return 0

