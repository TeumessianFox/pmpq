import subprocess
import logging
from datetime import datetime
import numpy as np


POLYMUL_ALGOS = ["TEXTBOOK_SIMPLE",
                 "TEXTBOOK_CLEAN",
                 "TEXTBOOK_CLEAN_4",
                 "TEXTBOOK_STATIC",
                 "KARATSUBA_ONLY",
                 "ASM_SCHOOLBOOK_24",
                 "POLYMUL_ASM_TOOM3_6_2",
                 "POLYMUL_CHAIN"]
CHAIN_OPTIONS = {"KARATSUBA": "karatsuba",
                 "TOOM-COOK-3": "toom_cook_3",
                 "ASM_SCHOOLBOOK_24": "remapped_schoolbook_24x24",
                 "TEXTBOOK": "remapped_textbook"}


class PolymulAlgo:
    def __init__(self, name: str, chain_size=0, chain=None, degree=0, opt='s'):
        if not name in POLYMUL_ALGOS:
            logging.critical("{} not in list of available polymul algos".format(name))
            exit(10)
        self.name = name
        self.chain_size = chain_size
        self.chain = chain
        self.degree = degree
        self.opt = "-O" + opt

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

    def flash(self):
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
            chain_str = '"{}"'.format("_".join([self.chain]))
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
