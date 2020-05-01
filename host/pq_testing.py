from numpy.random import RandomState
from numpy import uint16, zeros, uint64
import numpy as np
import m4serial
import subprocess
from datetime import datetime
import logging

# Streamlined NTRU Prime: sntrup4591761
P, Q, W = 761, 4591, 286

POLYMUL_ALGOS = ["TEXTBOOK", "ASM_SCHOOLBOOK_24", "KARATSUBA_ONLY", "POLYMUL_CHAIN"]
CHAIN_OPTIONS = {"KARATSUBA": "karatsuba",
                 "ASM_SCHOOLBOOK_24": "remapped_schoolbook_24x24",
                 "TEXTBOOK": "remapped_textbook"}


def init(algo: str, chain_size=0, chain=None):
    logging.info("#### Running {} ####".format(algo))
    if algo != "POLYMUL_CHAIN":
        make(algo)
    else:
        make(algo, chain_size, chain)
    flash()
    m4serial.init()


def make(algo: str, chain_size=0, chain=None):
    subprocess.run(["mkdir -p log"], shell=True)
    with open("log/make.log", 'w') as f:
        subprocess.run(["make clean -C ../m4/"], shell=True, stdout=f, text=True)
        if chain_size == 0:
            makecommand = "make POLYMUL={} -C ../m4/".format(algo)
        else:
            chain_str = '"{}"'.format(", ".join([CHAIN_OPTIONS[c] for c in chain]))
            makecommand = 'make POLYMUL={} CHAIN_SIZE={} CHAIN={} -C ../m4/'.format(algo, chain_size, chain_str)
        logging.info(makecommand)
        build = subprocess.run([makecommand], shell=True, stdout=f, text=True)
        if build.returncode != 0:
            logging.critical(makecommand)
            logging.critical("Use Python >= 3.5")
            exit(1)
    with open("log/make.log", 'r') as f:
        logging.debug(f.read())


def flash():
    with open("log/flash.log", 'w') as f:
        build = subprocess.run(["make flash -C ../m4/"], shell=True, stdout=f, stderr=f, text=True)
        if build.returncode != 0:
            logging.critical("make flash -C ../m4/")
            logging.critical("Check if board is connected")
            exit(1)
    with open("log/flash.log", 'r') as f:
        logging.debug(f.read())


def key_gen(seed):
    r = RandomState(seed)
    c_values = 2 * r.randint(2, size=W) - 1  # W "small" non-zero coefficients, i.e. -1, 1
    c_pos = r.choice(P, W, replace=False)  # choose W positions out of the P possible ones
    cs = np.zeros(P, dtype=uint16)
    for i in range(W):  # fill the W non-zero values
        cs[c_pos[i]] = c_values[i]
    return cs


def text_gen(seed):
    r = RandomState(seed)
    return (r.randint(0, Q, size=P) * 3 // 3) % Q


def log_to_file(algo: str, key_num, text_num, result, cycles):
    now = datetime.now()
    dt_string = now.strftime("%d.%m.%Y_%H:%M:%S")
    filename = "log/" + dt_string + "_" + algo.lower() + ".log"
    with open(filename, 'w') as f:
        logging.info("Logfile created: {}".format(filename))
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


def test_m4_pq(algo, key_num, text_num):
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
    cycles = uint16_to_uint64(cycles_uint16)
    logging.info("Cycles: " + str(cycles))
    output8 = m4serial.simpleserial_get('r')
    output = m4serial.uint8_to_uint16(output8)

    expected = np.zeros(shape=len(key_num) + len(text_num), dtype=uint16)
    for i in range(len(key_num)):
        for j in range(len(text_num)):
            expected[i + j] += key_num[i] * text_num[j]

    for i in range(len(output)):
        if output[i] != expected[i]:
            logging.critical("ERROR: Output not correct")
            logging.critical("{}: {} != {}".format(i, output[i], expected[i]))
            exit(1)
    logging.debug("Expected result and M4 result are equal")
    log_to_file(algo, key_num, text_num, output, cycles)
    logging.info("#### Run completed ####")
    m4serial.end()
    return output
