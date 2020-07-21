import pq_testing
import logging
import numpy as np
import os.path
import matplotlib.pyplot as plt


def textbook_eval(num_seeds=1):
    plt.close('all')
    seeds = np.random.default_rng().integers(0, 1000, size=num_seeds)
    logging.info("Seeds: {}".format(seeds))
    keys = list(map(pq_testing.key_gen, seeds))
    texts = list(map(pq_testing.text_gen, seeds))
    degree = range(12, 512, 4)

    cycles_textbook_simple = eval_algo("TEXTBOOK_SIMPLE", seeds, keys, texts, degree)

    plt.figure(1)
    plt.plot(degree, cycles_textbook_simple, color='b')
    plt.title("Simple textbook performance")
    plt.xlabel("Polynomial degree")
    plt.ylabel("Clock cycles")
    plt.tight_layout(pad=0.4, w_pad=0.5, h_pad=1.0)
    plt.savefig("results/textbook_simple.pdf", bbox_inches='tight')

    cycles_textbook_clean = eval_algo("TEXTBOOK_CLEAN", seeds, keys, texts, degree)
    cycles_textbook_clean_4 = eval_algo("TEXTBOOK_CLEAN_4", seeds, keys, texts, degree)

    fig, (ax1, ax2) = plt.subplots(2, 1, sharex=True, figsize=(6, 8))
    ax1.plot(degree, cycles_textbook_clean,   color='g', label="Simple textbook")
    ax1.plot(degree, cycles_textbook_clean_4, color='r', label="Four calculations per loop")
    ax1.set_ylabel("Clock cycles")
    ax1.set_yticks(np.arange(0, max(cycles_textbook_clean)+500000, 500000))
    ax1.legend()

    ax2.plot(degree, (1-cycles_textbook_clean_4/cycles_textbook_clean)*100)
    ax2.set_xlabel("Polynomial degree")
    ax2.set_ylabel("Speed-up in %")
    ax2.set_ylim(12, 20)

    fig.tight_layout(pad=0.4, w_pad=0.5, h_pad=1.0)
    fig.savefig("results/textbook_4_comparision.pdf", bbox_inches='tight')


def eval_algo(algo, seeds, keys, texts, degree):
    filename = "results/{}.npy".format(algo.lower())
    if os.path.isfile(filename):
        cycles_textbook = np.load(filename)
        logging.info("Loading {}".format(filename))
    else:
        pq_testing.init(algo, 0, [])
        cycles_textbook = []
        for i in degree:
            logging.info("Degree: {}".format(i))
            output_simple = 0
            for s, seed in enumerate(seeds):
                output_simple += pq_testing.test_m4_pq(algo, keys[s][0:i], texts[s][0:i], log=False)[1]
            cycles_textbook.append(output_simple / len(seeds))
        np.save(filename, cycles_textbook)
        logging.info(cycles_textbook)
    return cycles_textbook
