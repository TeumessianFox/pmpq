import serial
import subprocess
import time

BAUDRATE = 115200
DEVICE = "/dev/ttyUSB0"
MAX_LEN = 2048

ser = serial.Serial()


def init():
    make()
    flash()
    time.sleep(1)
    ser.baudrate = BAUDRATE
    ser.port = DEVICE
    ser.open()


def make():
    with open("make.debug", 'w') as f:
        build = subprocess.run(["make -C ../polymul/"], shell=True, stdout=f, text=True)
        if build.returncode != 0:
            print("Error in:")
            print("make -C ../polymul/")
            print("Use Python >= 3.5")
            exit(1)
    with open("make.debug", 'r') as f:
        print(f.read())


def flash():
    with open("flash.debug", 'w') as f:
        build = subprocess.run(["make flash -C ../polymul/"], shell=True, stdout=f, text=True)
        if build.returncode != 0:
            print("Error in:")
            print("make flash -C ../polymul/")
            print("Check if board is connected")
            exit(1)
    with open("make.debug", 'r') as f:
        print(f.read())


def utf8_to_int(twobyte):
    ascii_int = [int.from_bytes([twobyte[0]], byteorder="big"), int.from_bytes([twobyte[1]], byteorder="big")]
    for i in range(0, 2):
        if 48 <= ascii_int[i] <= 57:
            ascii_int[i] = ascii_int[i] - 48
        elif 65 <= ascii_int[i] <= 70:
            ascii_int[i] = ascii_int[i] - 55
        elif 97 <= ascii_int[i] <= 102:
            ascii_int[i] = ascii_int[i] - 87
    return ascii_int


def recombine_int(twoint):
    return (twoint[0] << 4) + twoint[1]


def simpleserial_get(c):
    recv_int = list()
    symbol = c.encode("utf-8")
    while True:
        c = ser.read(1)
        if c == b'd':
            string = ''
            last = ser.read(1)
            while last != b'\n':
                string += last.decode('utf-8')
                last = ser.read(1)
            print("DEBUG: " + string)
        if c == symbol:
            break
    end = 0
    for x in range(0, MAX_LEN):
        byteone = ser.read(1)
        if byteone == b'\n':
            end = 1
            break
        bytetwo = ser.read(1)
        print("{}: Received {}".format(x, byteone + bytetwo))
        recv_int.append(recombine_int(utf8_to_int(byteone + bytetwo)))
    if end != 1:
        c = ser.read(1)
        if c != b'\n':
            print("End of transmission '\n' not received")
    return recv_int


hex_lookup = [b'0', b'1', b'2', b'3', b'4', b'5', b'6', b'7', b'8', b'9', b'A', b'B', b'C', b'D', b'E', b'F']


def simpleserial_put(c: str, data):
    ser.write(c.encode("utf-8"))
    size = len(data)
    for i in range(0, size):
        ser.write(hex_lookup[data[i] >> 4])
        ser.write(hex_lookup[data[i] & 15])
    ser.write(b'\n')
