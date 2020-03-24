from numpy.random import RandomState
from numpy import uint16, zeros, uint64
import numpy as np
import m4serial
from datetime import datetime

# Streamlined NTRU Prime: sntrup4591761
P, Q, W = 761, 4591, 286


def key_gen(seed):
    # TODO see if this is a good approximation of the key distribution
    # this is supposed to give a random, small element of R,
    # R = Z[x]/(x**p−x−1), that is invertible in R/3.
    # TODO this does not check for invertibility
    r = RandomState(seed)
    c_values = 2 * r.randint(2, size=W) - 1  # W "small" non-zero coefficients, i.e. -1, 1
    c_pos = r.choice(P, W, replace=False)  # choose W positions out of the P possible ones
    cs = np.zeros(P, dtype=uint16)
    for i in range(W):  # fill the W non-zero values
        cs[c_pos[i]] = c_values[i]
    return cs


def text_gen(seed):
    # uniformly random element of R with coefficients being only
    # multiple of 3
    r = RandomState(seed)
    return (r.randint(0, Q, size=P) * 3 // 3) % Q


def log_to_file(key_num, text_num, result, cycles):
    now = datetime.now()
    dt_string = now.strftime("%d.%m.%Y_%H:%M:%S")
    filename = "log/" + dt_string + ".log"
    with open(filename, 'w') as f:
        f.write("Key:\n")
        f.write(np.array2string(key_num, separator=','))
        f.write("\n\n")
        f.write("Text:\n")
        f.write(np.array2string(text_num, separator=','))
        f.write("\n\n")
        f.write("Result:\n")
        f.write(np.array2string(result, separator=',', threshold=len(result)))
        f.write("\n\n")
        f.write("Cycles:\n")
        f.write(str(cycles))


def uint16_to_uint64(b: uint16):
    assert len(b) == 4
    nums: uint64 = uint64(b[3]*2**48) + uint64(b[2]*2**32) + uint64(b[1]*2**16) + uint64(b[0])
    return nums


def test_m4_pq(key_num, text_num):
    m4serial.simpleserial_put('k', key_num)
    result = m4serial.simpleserial_get('z')
    if result[0] == 0:
        print("M4: Key received")
    else:
        print("M4: Error when receiving the key")
    m4serial.simpleserial_put('p', text_num)
    result = m4serial.simpleserial_get('z')
    if result[0] == 0:
        print("M4: Plain text received")
    else:
        print("M4: Error when receiving the plain text")

    cycles_uit16 = m4serial.simpleserial_get('c')
    cycles = uint16_to_uint64(cycles_uit16)
    print("Cycles need: " + str(cycles))
    output = m4serial.simpleserial_get('r')

    expected = np.zeros(shape=len(key_num) + len(text_num), dtype=uint16)
    for i in range(len(key_num)):
        for j in range(len(text_num)):
            expected[i + j] += key_num[i] * text_num[j]

    assert len(output) == len(expected)
    for i in range(len(output)):
        if output[i] != expected[i]:
            print("ERROR: Output not correct")
            print("{}: {} != {}".format(i, output[i], expected[i]))
            exit(1)
    log_to_file(key_num, text_num, output, cycles)
    print("Expected result and M4 result are equal")
    return output
