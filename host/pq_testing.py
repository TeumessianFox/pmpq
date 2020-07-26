from numpy.random import RandomState
from numpy import uint16
import numpy as np
import m4serial
import logging
import pm_algo

# Streamlined NTRU Prime: sntrup4591761
P, Q, W = 761, 4591, 286


def key_gen_sntrup4591761(seed):
    r = RandomState(seed)
    c_values = 2 * r.randint(2, size=W) - 1  # W "small" non-zero coefficients, i.e. -1, 1
    c_pos = r.choice(P, W, replace=False)  # choose W positions out of the P possible ones
    cs = np.zeros(P, dtype=uint16)
    for i in range(W):  # fill the W non-zero values
        cs[c_pos[i]] = c_values[i]
    return cs


def text_gen_sntrup4591761(seed):
    return (np.random.default_rng(seed=seed).integers(0, Q, size=P) // 3) * 3


def test_m4_pq(algo: pm_algo, key_num, text_num, log=True):
    m4serial.init()
    m4serial.simpleserial_put('k', key_num)
    result = m4serial.simpleserial_get('z')
    if result[0] == 0:
        logging.debug("M4 received key")
    else:
        logging.critical("M4 Error when receiving the key")
        exit(1)
    m4serial.simpleserial_put('p', text_num)
    result = m4serial.simpleserial_get('z')
    if result[0] == 0:
        logging.debug("M4 received plain text")
    else:
        logging.critical("M4 Error when receiving the plain text")
        exit(1)

    cycles_uit8 = m4serial.simpleserial_get('c')
    cycles_uint16 = m4serial.uint8_to_uint16(cycles_uit8)
    cycles = m4serial.uint16_to_uint64(cycles_uint16)
    logging.info("Cycles: " + str(cycles))
    output8 = m4serial.simpleserial_get('r')
    output = m4serial.uint8_to_uint16(output8)

    expected = np.zeros(shape=len(key_num) + len(text_num) - 1, dtype=uint16)
    for i in range(len(key_num)):
        for j in range(len(text_num)):
            expected[i + j] += key_num[i] * text_num[j]

    counter = 0
    shift = (len(key_num) + 2) // 3
    chunk = 2 * shift - 1
    for i in range(len(output)):
        if output[i] != expected[i]:
        #if output[i] % 2 ** 15 != expected[i] % 2 ** 15:
            logging.critical("ERROR: Output not correct")
            logging.critical("{}: {} != {}".format(i, output[i], expected[i]))
            for num, wi in enumerate([0, 3, 1, 2, 4]):
                if num * shift <= i <= num * shift + (chunk - 1) and wi != 0 and wi != 4:
                    logging.critical("Error from W{}[{}] ".format(wi, i - num * shift))
            counter += 1
    if counter > 0:
        logging.critical("ERROR: {} values are wrong".format(counter))
    if log:
        algo.log_to_file(key_num, text_num, output, cycles)
    assert counter == 0
    logging.debug("Expected result and M4 result are equal")
    logging.info("#### Run completed ####")
    m4serial.simpleserial_put('x', expected)
    m4serial.end()
    return [output, cycles]
