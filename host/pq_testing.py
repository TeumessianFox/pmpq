from numpy.random import RandomState
from numpy import uint16
import numpy as np
import m4serial

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


def test_m4_pq(seed):
    key_num = key_gen(seed)
    text_num = text_gen(seed)
    m4serial.simpleserial_put('k', key_num)
    result = m4serial.simpleserial_get('z')
    if result[0] == 0:
        print("Key received")
    else:
        print("Error when receiving the key")
    m4serial.simpleserial_put('p', text_num)
    result = m4serial.simpleserial_get('z')
    if result[0] == 0:
        print("Plain text received")
    else:
        print("Error when receiving the plain text")

    output = m4serial.simpleserial_get('r')

    expected = np.zeros(shape=len(key_num) + len(text_num), dtype=uint16)
    for i in range(len(key_num)):
        for j in range(len(text_num)):
            expected[i + j] += key_num[i] * text_num[j]

    assert len(output) == len(expected)
    for i in range(len(output)):
        if output[i] != expected[i]:
            print("{}: {} != {}".format(i, output[i], expected[i]))
        assert output[i] == expected[i]
    print("Expected result and M4 result are equal")
