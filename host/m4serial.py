import serial
from numpy import zeros, uint16
import logging

BAUDRATE = 115200
DEVICE = "/dev/ttyUSB0"
MAX_LEN = 1024

ser = serial.Serial()


def init():
    ser.baudrate = BAUDRATE
    ser.port = DEVICE
    ser.open()


def end():
    ser.close()


def int16_to_bytes(arr):
    b = bytearray()
    for el in arr:
        assert -2**16 <= el < 2**16
        el %= 2**16
        b.append(el % 2**8)
        b.append(el // 2**8)
    return b


def bytes_to_int16(b):
    assert len(b) % 2 == 0
    nums = zeros(shape=len(b)//2, dtype=uint16)
    for i in range(0, len(b), 2):
        nums[i//2] = int(b[i+1]*2**8) + int(b[i])
    return nums


def utf8_to_byte(twobyte):
    ascii_int = [int.from_bytes([twobyte[0]], byteorder="big"), int.from_bytes([twobyte[1]], byteorder="big")]
    for i in range(0, 2):
        if 48 <= ascii_int[i] <= 57:
            ascii_int[i] = ascii_int[i] - 48
        elif 65 <= ascii_int[i] <= 70:
            ascii_int[i] = ascii_int[i] - 55
        elif 97 <= ascii_int[i] <= 102:
            ascii_int[i] = ascii_int[i] - 87
    return ascii_int


def recombine_byte(twoint):
    assert len(twoint) == 2
    return (twoint[0] << 4) + twoint[1]


def uint8_to_uint16(recv_uint8):
    assert len(recv_uint8) % 2 == 0
    recv_uint16 = zeros(shape=len(recv_uint8)//2, dtype=uint16)
    for x in range(len(recv_uint8)//2):
        recv_uint16[x] = bytes_to_int16([recv_uint8[2*x], recv_uint8[2*x+1]])[0]
    return recv_uint16


def simpleserial_get(c: str):
    symbol = c.encode("utf-8")
    while True:
        c = ser.read(1)
        if c == b'd':
            string = ''
            last = ser.read(1)
            while last != b'\n':
                string += last.decode('utf-8')
                last = ser.read(1)
            logging.info("M4: " + string)
        if c == symbol:
            break
    end = 0
    recv_int8 = list()
    for x in range(0, 4*MAX_LEN):
        byteone = ser.read(1)
        if byteone == b'\n':
            end = 1
            break
        bytetwo = ser.read(1)
        recv_int8.append(recombine_byte(utf8_to_byte(byteone + bytetwo)))
    if end != 1:
        c = ser.read(1)
        if c != b'\n':
            logging.error("End of transmission '\n' not received")
    return recv_int8


hex_lookup = [b'0', b'1', b'2', b'3', b'4', b'5', b'6', b'7', b'8', b'9', b'A', b'B', b'C', b'D', b'E', b'F']


def simpleserial_put(c: str, data):
    ser.write(c.encode("utf-8"))
    nums = int16_to_bytes(data)
    size = len(nums)
    for i in range(0, size):
        ser.write(hex_lookup[nums[i] >> 4])
        ser.write(hex_lookup[nums[i] & 15])
    ser.write(b'\n')
