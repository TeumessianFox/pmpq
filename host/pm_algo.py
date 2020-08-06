import subprocess
from datetime import datetime
import numpy as np
from numpy import uint16
import m4serial
import logging

POLYMUL_ALGOS = ["TEXTBOOK_SIMPLE",
                 "TEXTBOOK_CLEAN",
                 "TEXTBOOK_CLEAN_4",
                 "TEXTBOOK_STATIC",
                 "KARATSUBA_ONLY",
                 "ASM_SCHOOLBOOK_12",
                 "ASM_SCHOOLBOOK_16",
                 "ASM_SCHOOLBOOK_24",
                 "POLYMUL_CHAIN"]
CHAIN_OPTIONS = {"KARATSUBA": "karatsuba",
                 "TOOM-COOK-3": "toom_cook_3",
                 "TOOM-COOK-3_LIBPOLYMATH": "toom_cook_3_libpolymath",
                 "ASM_SCHOOLBOOK_12": "remapped_schoolbook_12x12",
                 "ASM_SCHOOLBOOK_16": "remapped_schoolbook_16x16",
                 "ASM_SCHOOLBOOK_24": "remapped_schoolbook_24x24",
                 "TEXTBOOK": "remapped_textbook"}
OPT_OPTIONS = {'0', '1', '2', '3', 's'}


class PolymulAlgo:
    def __init__(self, name: str, chain_size=0, chain=None, degree=0, opt='s'):
        if name not in POLYMUL_ALGOS:
            logging.critical("{} not in list of available polymul algos".format(name))
            exit(1)
        self.name = name
        self.chain_size = chain_size
        self.chain = chain
        self.degree = degree
        if opt not in OPT_OPTIONS:
            logging.critical("-O{} not a possible optimization option".format(opt))
            exit(1)
        self.opt = "-O" + opt
        if self.name == "POLYMUL_CHAIN":
            self.toom3count = self.chain.count("TOOM-COOK-3") + self.chain.count("TOOM-COOK-3_LIBPOLYMATH")
            logging.info("Polymul chain includes {} Toom-Cook-3. All results are only {} bits precise".format(self.toom3count, 16 - self.toom3count))

    def build(self):
        logging.info("#### Running {} ####".format(self.name))
        self.make()
        self.flash()

    def make(self):
        subprocess.run(["mkdir -p log"], shell=True)
        with open("log/make.log", 'w') as f:
            subprocess.run(["make clean -C ../m4/"], shell=True, stdout=f, text=True)
            makecommand = "make POLYMUL={} ".format(self.name)
            if self.chain_size != 0:
                chain_str = '"{}"'.format(", ".join([CHAIN_OPTIONS[c] for c in self.chain]))
                makecommand += "CHAIN_SIZE={} CHAIN={} ".format(self.chain_size, chain_str)
            makecommand += "DEGREE={} ".format(self.degree)
            makecommand += "OPT={} ".format(self.opt)
            makecommand += "-C ../m4/"
            logging.info(makecommand)
            build = subprocess.run([makecommand], shell=True, stdout=f, text=True)
            if build.returncode != 0:
                logging.critical(makecommand)
                logging.critical("Use Python >= 3.7")
                exit(1)
        with open("log/make.log", 'r') as f:
            logging.debug(f.read())

    @staticmethod
    def flash() -> None:
        with open("log/flash.log", 'w') as f:
            build = subprocess.run(["make flash -C ../m4/"], shell=True, stdout=f, stderr=f, text=True)
            if build.returncode != 0:
                logging.critical("make flash -C ../m4/")
                logging.critical("Check if board is connected")
                exit(1)
        with open("log/flash.log", 'r') as f:
            logging.debug(f.read())

    def log_to_file(self, key_num, text_num, result, cycles):
        now = datetime.now()
        dt_string = now.strftime("%d.%m.%Y_%H:%M:%S")
        filename = "log/" + dt_string + "_"
        if self.name == "POLYMUL_CHAIN" and self.chain_size != 0:
            chain_str = '{}'.format("_".join([link.lower() for link in self.chain]))
            filename += chain_str
        else:
            filename += self.name.lower()
        filename += ".log"
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

    def run_polymul(self, key, text, log=True):
        m4serial.init()

        # Send key and wait for ACK
        m4serial.simpleserial_put('k', key)
        ack = m4serial.simpleserial_get('z')
        if ack[0] == 0:
            logging.debug("M4 received key")
        else:
            logging.critical("M4 Error when receiving the key")
            exit(1)

        # Send plain text and wait for ACK
        m4serial.simpleserial_put('p', text)
        ack = m4serial.simpleserial_get('z')
        if ack[0] == 0:
            logging.debug("M4 received plain text")
        else:
            logging.critical("M4 Error when receiving the plain text")
            exit(1)

        # Gather clock cycle count
        cycles_uint8 = m4serial.simpleserial_get('c')
        cycles_uint16 = m4serial.uint8_to_uint16(cycles_uint8)
        cycles = m4serial.uint16_to_uint64(cycles_uint16)
        logging.info("Cycles: " + str(cycles))

        # Receive result
        output8 = m4serial.simpleserial_get('r')
        output = m4serial.uint8_to_uint16(output8)

        # Calculate expected result using slow textbook polymul and compare it to the received output
        expected = np.zeros(shape=len(key) + len(text) - 1, dtype=uint16)
        for i in range(len(key)):
            for j in range(len(text)):
                expected[i + j] += key[i] * text[j]
        counter = 0
        for i in range(len(output)):
            if self.name == "POLYMUL_CHAIN":
                if output[i] % 2 ** (16 - self.toom3count) != expected[i] % 2 ** (16 - self.toom3count):
                    logging.critical("ERROR: Output not correct")
                    logging.critical("{}: {} != {}".format(i, output[i], expected[i]))
                    counter += 1
            elif output[i] != expected[i]:
                logging.critical("ERROR: Output not correct")
                logging.critical("{}: {} != {}".format(i, output[i], expected[i]))
                counter += 1
        try:
            assert counter == 0
        except AssertionError as err:
            logging.critical("ERROR: {} values are wrong".format(counter))
            raise err

        if log:
            self.log_to_file(key, text, output, cycles)
        logging.debug("Expected result and M4 result are equal")

        # Send reset signal
        m4serial.simpleserial_put('x', np.zeros(shape=2, dtype=uint16))
        result = m4serial.simpleserial_get('z')
        if result[0] == 0:
            logging.debug("M4 received reset ack")
        else:
            logging.critical("M4 Error when receiving reset ack")
            exit(1)

        logging.info("#### Run successful ####")
        m4serial.end()
        return [output, cycles]
