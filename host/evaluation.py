import generate
from pm_algo import PolymulAlgo
import logging
import numpy as np
import os.path
import matplotlib.pyplot as plt
import pm_root


class Evaluation:
    def __init__(self, num_seeds=1):
        plt.close('all')
        self.seeds = np.random.default_rng().integers(0, 1000, size=num_seeds)
        logging.info("Seeds: {}".format(self.seeds))
        random_key_text = list(map(generate.random_gen, self.seeds))
        self.keys, self.texts = [r[0] for r in random_key_text], [r[1] for r in random_key_text]

    def evaluate_all(self):
        self.textbook_eval()
        self.karatsuba_eval()
        self.toom_3_eval()
        self.schoolbook_eval()
        self.evaluate_polymul_chain()

    def evaluate_polymul_chain(self):
        plt.close('all')
        schoolbook_sequences, all_schoolbooks_chains = pm_root.polymul_chains()

        all_cycle_list = []
        all_degree_list = []
        for schoolbook_num, schoolbook_chains in enumerate(all_schoolbooks_chains):
            schoolbook_cycles_list = []
            schoolbook_degree_list = []
            for seq_num, sequence in enumerate(schoolbook_chains):
                degree = max(schoolbook_sequences[schoolbook_num][seq_num])
                algo = PolymulAlgo("POLYMUL_CHAIN", len(sequence), sequence)
                cycles_algo = self.eval_algo(algo, dir="polymul_chain/", degree=[degree])[0]
                schoolbook_cycles_list.append(cycles_algo)
                schoolbook_degree_list.append(degree)
            all_cycle_list.append(schoolbook_cycles_list)
            all_degree_list.append(schoolbook_degree_list)

        degree_4 = range(12, 1025, 4)
        algo_clean = PolymulAlgo("TEXTBOOK_CLEAN")
        cycles_textbook_clean = self.eval_algo(algo_clean, degree_4)

        plt.figure(1)
        for i, schoolbook in enumerate(pm_root.SCHOOLBOOKS):
            plt.plot(all_degree_list[i], all_cycle_list[i], marker='x', ls='',
                     label="Schoolbook {}x{}".format(schoolbook, schoolbook))
        plt.xlabel("Polynomial degree")
        plt.ylabel("Clock cycles")
        plt.legend()
        plt.ticklabel_format(axis='y', style='sci', scilimits=(6, 6))
        plt.tight_layout(pad=0.4, w_pad=0.5, h_pad=1.0)
        plt.savefig("results/schoolbooks_chains.pdf", bbox_inches='tight')

        plt.figure(2)
        plt.plot(degree_4, cycles_textbook_clean, color='r', label="Simple textbook")
        for i, schoolbook in enumerate(pm_root.SCHOOLBOOKS):
            plt.plot(all_degree_list[i], all_cycle_list[i], marker='x', ls='',
                     label="Schoolbook {}x{}".format(schoolbook, schoolbook))
        plt.xlabel("Polynomial degree")
        plt.ylabel("Clock cycles")
        plt.legend()
        plt.ticklabel_format(axis='y', style='sci', scilimits=(6, 6))
        plt.tight_layout(pad=0.4, w_pad=0.5, h_pad=1.0)
        plt.savefig("results/schoolbook_chain_vs_textbook.pdf", bbox_inches='tight')

        for i, schoolbook in enumerate(pm_root.SCHOOLBOOKS):
            plt.figure(i + 10)
            plt.plot(all_degree_list[i], all_cycle_list[i], marker='x', ls='',
                     label="Schoolbook {}x{}".format(schoolbook, schoolbook))
            plt.xlabel("Polynomial degree")
            plt.ylabel("Clock cycles")
            plt.ticklabel_format(axis='y', style='sci', scilimits=(6, 6))
            plt.legend()

            plt.tight_layout(pad=0.4, w_pad=0.5, h_pad=1.0)
            plt.savefig("results/schoolbook_chains_{}.pdf".format(schoolbook), bbox_inches='tight')

    def schoolbook_eval(self):
        plt.close('all')
        key, text = self.keys[0][0:24], self.texts[0][0:24]
        filename = "results/np_save/schoolbook.npy"
        if os.path.isfile(filename):
            cycles = np.load(filename)
            logging.info("Loading {}".format(filename))
        else:
            algo_schoolbook_24 = PolymulAlgo("ASM_SCHOOLBOOK_24")
            algo_schoolbook_24.build()
            cycles_schoolbook_24 = algo_schoolbook_24.run_polymul(key, text, log=False)[1]

            algo_schoolbook_16 = PolymulAlgo("ASM_SCHOOLBOOK_16")
            algo_schoolbook_16.build()
            cycles_schoolbook_16 = algo_schoolbook_16.run_polymul(key[0:16], text[0:16], log=False)[1]

            algo_schoolbook_12 = PolymulAlgo("ASM_SCHOOLBOOK_12")
            algo_schoolbook_12.build()
            cycles_schoolbook_12 = algo_schoolbook_12.run_polymul(key[0:12], text[0:12], log=False)[1]

            algo_clean = PolymulAlgo("TEXTBOOK_CLEAN")
            algo_clean.build()
            cycles_textbook_clean_12 = algo_clean.run_polymul(key[0:12], text[0:12], log=False)[1]
            cycles_textbook_clean_16 = algo_clean.run_polymul(key[0:16], text[0:16], log=False)[1]
            cycles_textbook_clean_24 = algo_clean.run_polymul(key[0:24], text[0:24], log=False)[1]

            cycles = np.array([[cycles_schoolbook_12, cycles_schoolbook_16, cycles_schoolbook_24],
                               [cycles_textbook_clean_12, cycles_textbook_clean_16, cycles_textbook_clean_24]])
            np.save(filename, cycles)
        workload = np.array([12 * 12, 16 * 16, 24 * 24])
        latency = cycles / workload

        logging.info("Textbook 24x24: {} (Latency: {:.2f})".format(cycles[1][2], latency[1][2]))
        logging.info("Schoolbook 24x24: {} (Latency: {:.2f})".format(cycles[0][2], latency[0][2]))
        logging.info(
            "Difference: {} (Speedup: {:.2f}x)".format(cycles[1][2] - cycles[0][2], cycles[1][2] / cycles[0][2]))

        logging.info("Textbook 16x16: {} (Latency: {:.2f})".format(cycles[1][1], latency[1][1]))
        logging.info("Schoolbook 16x16: {} (Latency: {:.2f})".format(cycles[0][1], latency[0][1]))
        logging.info(
            "Difference: {} (Speedup: {:.2f}x)".format(cycles[1][1] - cycles[0][1], cycles[1][1] / cycles[0][1]))

        logging.info("Textbook 12x12: {} (Latency: {:.2f})".format(cycles[1][0], latency[1][0]))
        logging.info("Schoolbook 12x12: {} (Latency: {:.2f})".format(cycles[0][0], latency[0][0]))
        logging.info(
            "Difference: {} (Speedup: {:.2f}x)".format(cycles[1][0] - cycles[0][0], cycles[1][0] / cycles[0][0]))

    def toom_3_eval(self):
        plt.close('all')
        degree_6 = range(12, 1025, 6)
        degree_4 = range(12, 1025, 4)

        algo_toom_3_libpolymath_textbook = PolymulAlgo("POLYMUL_CHAIN", 2, ["TOOM-COOK-3_LIBPOLYMATH", "TEXTBOOK"])
        cycles_toom_3_libpolymath_textbook = self.eval_algo(algo_toom_3_libpolymath_textbook, degree_6)
        algo_toom_3_textbook = PolymulAlgo("POLYMUL_CHAIN", 2, ["TOOM-COOK-3", "TEXTBOOK"])
        cycles_toom_3_textbook = self.eval_algo(algo_toom_3_textbook, degree_6)

        plt.figure(1)
        plt.plot(degree_6, cycles_toom_3_libpolymath_textbook, color='g',
                 label="Toom-Cook-3 with libpolymath, textbook")
        plt.plot(degree_6, cycles_toom_3_textbook, color='b', label="Toom-Cook-3, textbook")
        plt.xlabel("Polynomial degree")
        plt.ylabel("Clock cycles")
        plt.legend()
        plt.ticklabel_format(axis='y', style='sci', scilimits=(6, 6))
        plt.tight_layout(pad=0.4, w_pad=0.5, h_pad=1.0)
        plt.savefig("results/toom_3_libpolymath.pdf", bbox_inches='tight')

        algo_clean = PolymulAlgo("TEXTBOOK_CLEAN")
        cycles_textbook_clean = self.eval_algo(algo_clean, degree_4)
        algo_karatsuba_textbook = PolymulAlgo("POLYMUL_CHAIN", 2, ["KARATSUBA", "TEXTBOOK"])
        cycles_karatsuba_textbook = self.eval_algo(algo_karatsuba_textbook, degree_4)
        algo_toom_3_textbook = PolymulAlgo("POLYMUL_CHAIN", 2, ["TOOM-COOK-3", "TEXTBOOK"])
        cycles_toom_3_textbook = self.eval_algo(algo_toom_3_textbook, degree_6)

        plt.figure(2)
        plt.plot(degree_4, cycles_textbook_clean, color='g', label="Simple textbook")
        plt.plot(degree_4, cycles_karatsuba_textbook, color='r', label="Karatsuba, textbook")
        plt.plot(degree_6, cycles_toom_3_textbook, color='b', label="Toom-Cook-3, textbook")
        plt.xlabel("Polynomial degree")
        plt.ylabel("Clock cycles")
        plt.legend()
        plt.ticklabel_format(axis='y', style='sci', scilimits=(6, 6))
        plt.tight_layout(pad=0.4, w_pad=0.5, h_pad=1.0)
        plt.savefig("results/toom_3.pdf", bbox_inches='tight')

    def karatsuba_eval(self):
        plt.close('all')
        degree_4 = range(12, 1025, 4)
        degree_power_2 = [2 ** j for j in range(3, 10 + 1)]

        algo_clean = PolymulAlgo("TEXTBOOK_CLEAN")
        cycles_textbook_clean = self.eval_algo(algo_clean, degree_4)
        algo_karatsuba_only = PolymulAlgo("KARATSUBA_ONLY")
        cycles_karatsuba_only = self.eval_algo(algo_karatsuba_only, degree_power_2)

        plt.figure(1)
        plt.plot(degree_power_2, cycles_karatsuba_only, color='b', marker='x', label="Recursive Karatsuba")
        plt.plot(degree_4, cycles_textbook_clean, color='g', label="Simple textbook")
        plt.xlabel("Polynomial degree")
        plt.ylabel("Clock cycles")
        plt.legend()
        plt.ticklabel_format(axis='y', style='sci', scilimits=(6, 6))
        plt.tight_layout(pad=0.4, w_pad=0.5, h_pad=1.0)
        plt.savefig("results/karatsuba_only.pdf", bbox_inches='tight')

        algo_karatsuba_textbook = PolymulAlgo("POLYMUL_CHAIN", 2, ["KARATSUBA", "TEXTBOOK"])
        cycles_karatsuba_textbook = self.eval_algo(algo_karatsuba_textbook, degree_4)

        plt.figure(2)
        plt.plot(degree_power_2, cycles_karatsuba_only, color='b', marker='x', label="Recursive Karatsuba")
        plt.plot(degree_4, cycles_textbook_clean, color='g', label="Simple textbook")
        plt.plot(degree_4, cycles_karatsuba_textbook, color='r', label="Karatsuba, textbook")
        plt.xlabel("Polynomial degree")
        plt.ylabel("Clock cycles")
        plt.legend()
        plt.ticklabel_format(axis='y', style='sci', scilimits=(6, 6))
        plt.tight_layout(pad=0.4, w_pad=0.5, h_pad=1.0)
        plt.savefig("results/karatsuba_textbook.pdf", bbox_inches='tight')

    def textbook_eval(self):
        plt.close('all')
        degree = range(12, 1025, 4)
        degree_16 = range(12, 1025, 16)

        algo_simple = PolymulAlgo("TEXTBOOK_SIMPLE")
        cycles_textbook_simple = self.eval_algo(algo_simple, degree)

        plt.figure(1)
        plt.plot(degree, cycles_textbook_simple, color='b')
        plt.xlabel("Polynomial degree")
        plt.ylabel("Clock cycles")
        plt.ticklabel_format(axis='y', style='sci', scilimits=(6, 6))
        plt.tight_layout(pad=0.4, w_pad=0.5, h_pad=1.0)
        plt.savefig("results/textbook_simple.pdf", bbox_inches='tight')

        algo_clean = PolymulAlgo("TEXTBOOK_CLEAN")
        cycles_textbook_clean = self.eval_algo(algo_clean, degree)
        algo_clean_4 = PolymulAlgo("TEXTBOOK_CLEAN_4")
        cycles_textbook_clean_4 = self.eval_algo(algo_clean_4, degree)

        plt.figure(1)
        plt.plot(degree, cycles_textbook_clean, color='g', label="Simple textbook")
        plt.plot(degree, cycles_textbook_clean_4, color='b', label="Four calculations per loop")
        plt.xlabel("Polynomial degree")
        plt.ylabel("Clock cycles")
        plt.legend()
        plt.ticklabel_format(axis='y', style='sci', scilimits=(6, 6))
        plt.tight_layout(pad=0.4, w_pad=0.5, h_pad=1.0)
        plt.savefig("results/textbook_4_comparision.pdf", bbox_inches='tight')

        algo_static = PolymulAlgo("TEXTBOOK_STATIC", opt='3')
        cycles_textbook_static = self.eval_algo(algo_static, degree_16, rebuild=True)

        plt.figure(3)
        plt.plot(degree, cycles_textbook_clean, color='g', label="Simple textbook")
        plt.plot(degree, cycles_textbook_clean_4, color='b', label="Four calculations per loop")
        plt.plot(degree_16, cycles_textbook_static, color='r', label="Static textbook")
        plt.xlabel("Polynomial degree")
        plt.ylabel("Clock cycles")
        plt.legend()
        plt.ticklabel_format(axis='y', style='sci', scilimits=(6, 6))
        plt.tight_layout(pad=0.4, w_pad=0.5, h_pad=1.0)
        plt.savefig("results/textbook_static.pdf", bbox_inches='tight')

    def eval_algo(self, algo: PolymulAlgo, degree, dir="", rebuild=False):
        filename = "results/np_save/"
        filename += dir
        if algo.name == "POLYMUL_CHAIN" and algo.chain_size != 0:
            chain_str = '{}'.format("_".join([link.lower() for link in algo.chain]))
            filename += chain_str
        else:
            filename += algo.name.lower()
        filename += ".npy"
        if os.path.isfile(filename):
            cycles_textbook = np.load(filename)
            logging.info("Loading {}".format(filename))
        else:
            algo.build()
            cycles_textbook = []
            for i in degree:
                if rebuild:
                    algo.degree = i
                    algo.build()
                logging.info("Degree: {}".format(i))
                output_simple = 0
                for s, seed in enumerate(self.seeds):
                    output_simple += algo.run_polymul(self.keys[s][0:i], self.texts[s][0:i], log=False)[1]
                cycles_textbook.append(int(output_simple // len(self.seeds)))
            np.save(filename, cycles_textbook)
            logging.info(cycles_textbook)
        return cycles_textbook
