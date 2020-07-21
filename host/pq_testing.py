from numpy.random import RandomState
from numpy import uint16, zeros, uint64
import numpy as np
import m4serial
import subprocess
from datetime import datetime
import logging

# Streamlined NTRU Prime: sntrup4591761
P, Q, W = 761, 4591, 286

POLYMUL_ALGOS = ["TEXTBOOK_SIMPLE",
                 "TEXTBOOK_CLEAN",
                 "TEXTBOOK_CLEAN_4",
                 "TEXTBOOK_STATIC",
                 "ASM_SCHOOLBOOK_24",
                 "POLYMUL_ASM_TOOM3_6_2",
                 "KARATSUBA_ONLY",
                 "POLYMUL_CHAIN"]
CHAIN_OPTIONS = {"KARATSUBA": "karatsuba",
                 "TOOM-COOK-3": "toom_cook_3",
                 "ASM_SCHOOLBOOK_24": "remapped_schoolbook_24x24",
                 "TEXTBOOK": "remapped_textbook_clean"}


def init(algo: str, chain_size=0, chain=None):
    logging.info("#### Running {} ####".format(algo))
    if algo != "POLYMUL_CHAIN":
        make(algo)
    else:
        make(algo, chain_size, chain)
    flash()


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
            logging.critical("Use Python >= 3.7")
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
    return (np.random.default_rng(seed=seed).integers(0, Q, size=P) // 3) * 3


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


def test_m4_pq(algo, key_num, text_num, log=True):
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
    cycles = uint16_to_uint64(cycles_uint16)
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
        log_to_file(algo, key_num, text_num, output, cycles)
    assert counter == 0
    logging.debug("Expected result and M4 result are equal")
    logging.info("#### Run completed ####")
    m4serial.simpleserial_put('x', expected)
    m4serial.end()
    return [output, cycles]
