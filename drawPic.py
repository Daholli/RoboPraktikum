import serial, time
import numpy as np

with open("./datapoints.txt", 'r') as f:
    content = f.readline()

convert = str(content).replace('(', '').replace(')','')
convert = str(convert).replace('[', '').replace(']', '')

data = np.fromstring(convert, dtype=int, sep=',')
data = data.reshape(int(data.shape[0]/2),2)
data = data.astype(str)

rcarriage = np.full((data.shape[0]), '\r', dtype="<U11")

x = np.full((data.shape[0]),"x", dtype="<U11")
y = np.full((data.shape[0]),"y", dtype="<U11")

data[:,0] = np.char.add(x,data[:,0])
data[:,0] = np.char.add(data[:,0], rcarriage)
data[:,1] = np.char.add(y,data[:,1])
data[:,1] = np.char.add(data[:,1], rcarriage)

#print(data)

class ScaraRobo():
    def __init__(self):
        self.ser = serial.Serial()
        self.ser.port = "/dev/ttyUSB0"
        self.ser.baudrate = 9600
        self.ser.bytesize = serial.EIGHTBITS
        self.ser.parity = serial.PARITY_NONE
        self.ser.stopbits = serial.STOPBITS_ONE

        try:
            self.ser.open()
        except Exception as e:
            print("error open serial port: " + str(e))
            exit()
        self.ser.write(chr(255).encode())

    def setX(self, line):
        self.ser.write(data[line][0].encode())

    def setY(self, line):
        self.ser.write(data[line][1].encode())


    def draw(self, data):
        for i in range(data.shape[0]):
            self.setX(i)
            self.setY(i)


robo=ScaraRobo()

robo.draw(data)
