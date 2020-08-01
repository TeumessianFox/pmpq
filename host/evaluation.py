import pq_testing
from pm_algo import PolymulAlgo
import logging
import numpy as np
import os.path
import matplotlib.pyplot as plt


def toom_3_eval(num_seeds=1):
    plt.close('all')
    seeds = np.random.default_rng().integers(0, 1000, size=num_seeds)
    logging.info("Seeds: {}".format(seeds))
    random_key_text = list(map(pq_testing.random_gen, seeds))
    keys, texts = [r[0] for r in random_key_text], [r[1] for r in random_key_text]
    degree_6 = range(12, 1025, 6)
    degree_4 = range(12, 1025, 4)

    algo_toom_3_libpolymath_textbook = PolymulAlgo("POLYMUL_CHAIN", 2, ["TOOM-COOK-3_LIBPOLYMATH", "TEXTBOOK"])
    cycles_toom_3_libpolymath_textbook = eval_algo(algo_toom_3_libpolymath_textbook, seeds, keys, texts, degree_6)
    algo_toom_3_textbook = PolymulAlgo("POLYMUL_CHAIN", 2, ["TOOM-COOK-3", "TEXTBOOK"])
    cycles_toom_3_textbook = eval_algo(algo_toom_3_textbook, seeds, keys, texts, degree_6)

    plt.figure(1)
    plt.plot(degree_6, cycles_toom_3_libpolymath_textbook, color='g', label="Toom-Cook-3 with libpolymath, textbook")
    plt.plot(degree_6, cycles_toom_3_textbook, color='b', label="Toom-Cook-3, textbook")
    plt.xlabel("Polynomial degree")
    plt.ylabel("Clock cycles")
    plt.legend()
    plt.ticklabel_format(axis='y', style='sci', scilimits=(6, 6))
    plt.tight_layout(pad=0.4, w_pad=0.5, h_pad=1.0)
    plt.savefig("results/toom_3_libpolymath.pdf", bbox_inches='tight')

    algo_clean = PolymulAlgo("TEXTBOOK_CLEAN")
    cycles_textbook_clean = eval_algo(algo_clean, seeds, keys, texts, degree_4)
    algo_karatsuba_textbook = PolymulAlgo("POLYMUL_CHAIN", 2, ["KARATSUBA", "TEXTBOOK"])
    cycles_karatsuba_textbook = eval_algo(algo_karatsuba_textbook, seeds, keys, texts, degree_4)
    algo_toom_3_textbook = PolymulAlgo("POLYMUL_CHAIN", 2, ["TOOM-COOK-3", "TEXTBOOK"])
    cycles_toom_3_textbook = eval_algo(algo_toom_3_textbook, seeds, keys, texts, degree_6)

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


def karatsuba_eval(num_seeds=1):
    plt.close('all')
    seeds = np.random.default_rng().integers(0, 1000, size=num_seeds)
    logging.info("Seeds: {}".format(seeds))
    random_key_text = list(map(pq_testing.random_gen, seeds))
    keys, texts = [r[0] for r in random_key_text], [r[1] for r in random_key_text]
    degree_4 = range(12, 1025, 4)
    degree_power_2 = [2 ** j for j in range(3, 10 + 1)]

    algo_clean = PolymulAlgo("TEXTBOOK_CLEAN")
    cycles_textbook_clean = eval_algo(algo_clean, seeds, keys, texts, degree_4)
    algo_karatsuba_only = PolymulAlgo("KARATSUBA_ONLY")
    cycles_karatsuba_only = eval_algo(algo_karatsuba_only, seeds, keys, texts, degree_power_2)

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
    cycles_karatsuba_textbook = eval_algo(algo_karatsuba_textbook, seeds, keys, texts, degree_4)

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


def textbook_eval(num_seeds=1):
    plt.close('all')
    seeds = np.random.default_rng().integers(0, 1000, size=num_seeds)
    logging.info("Seeds: {}".format(seeds))
    random_key_text = list(map(pq_testing.random_gen, seeds))
    keys, texts = [r[0] for r in random_key_text], [r[1] for r in random_key_text]
    degree = range(12, 1025, 4)
    degree_16 = range(12, 1025, 16)

    algo_simple = PolymulAlgo("TEXTBOOK_SIMPLE")
    cycles_textbook_simple = eval_algo(algo_simple, seeds, keys, texts, degree)

    plt.figure(1)
    plt.plot(degree, cycles_textbook_simple, color='b')
    plt.xlabel("Polynomial degree")
    plt.ylabel("Clock cycles")
    plt.ticklabel_format(axis='y', style='sci', scilimits=(6, 6))
    plt.tight_layout(pad=0.4, w_pad=0.5, h_pad=1.0)
    plt.savefig("results/textbook_simple.pdf", bbox_inches='tight')

    algo_clean = PolymulAlgo("TEXTBOOK_CLEAN")
    cycles_textbook_clean = eval_algo(algo_clean, seeds, keys, texts, degree)
    algo_clean_4 = PolymulAlgo("TEXTBOOK_CLEAN_4")
    cycles_textbook_clean_4 = eval_algo(algo_clean_4, seeds, keys, texts, degree)

    fig, (ax1, ax2) = plt.subplots(2, 1, sharex=True, figsize=(6, 8))
    ax1.plot(degree, cycles_textbook_clean,   color='g', label="Simple textbook")
    ax1.plot(degree, cycles_textbook_clean_4, color='b', label="Four calculations per loop")
    ax1.set_ylabel("Clock cycles")
    ax1.set_yticks(np.arange(0, max(cycles_textbook_clean)+1000000, 1000000))
    ax1.ticklabel_format(axis='y', style='sci', scilimits=(6, 6))
    ax1.legend()

    ax2.plot(degree, (1-cycles_textbook_clean_4/cycles_textbook_clean)*100)
    ax2.set_xlabel("Polynomial degree")
    ax2.set_ylabel("Speed-up in %")
    ax2.set_ylim(12, 20)

    fig.tight_layout(pad=0.4, w_pad=0.5, h_pad=1.0)
    fig.savefig("results/textbook_4_comparision.pdf", bbox_inches='tight')

    algo_static = PolymulAlgo("TEXTBOOK_STATIC", opt='3')
    cycles_textbook_static = eval_algo(algo_static, seeds, keys, texts, degree_16, rebuild=True)

    plt.figure(3)
    plt.plot(degree, cycles_textbook_clean,   color='g', label="Simple textbook")
    plt.plot(degree, cycles_textbook_clean_4, color='b', label="Four calculations per loop")
    plt.plot(degree_16, cycles_textbook_static, color='r', label="Static textbook")
    plt.xlabel("Polynomial degree")
    plt.ylabel("Clock cycles")
    plt.legend()
    plt.ticklabel_format(axis='y', style='sci', scilimits=(6, 6))
    plt.tight_layout(pad=0.4, w_pad=0.5, h_pad=1.0)
    plt.savefig("results/textbook_static.pdf", bbox_inches='tight')


def eval_algo(algo: PolymulAlgo, seeds, keys, texts, degree, rebuild=False):
    filename = "results/np_save/"
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
            for s, seed in enumerate(seeds):
                output_simple += algo.run_polymul(keys[s][0:i], texts[s][0:i], log=False)[1]
            cycles_textbook.append(output_simple / len(seeds))
        np.save(filename, cycles_textbook)
        logging.info(cycles_textbook)
    return cycles_textbook
