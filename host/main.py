import pq_testing
import logging
import evaluation
from pm_algo import PolymulAlgo, POLYMUL_ALGOS


def main():
    evaluation.textbook_eval()
    evaluation.karatsuba_eval()
    # test_algos()


def test_algos():
    seed = 168
    logging.info("Seed: {}".format(seed))
    key_num = pq_testing.key_gen_sntrup4591761(seed)
    text_num = pq_testing.text_gen_sntrup4591761(seed)
    for algo in POLYMUL_ALGOS:
        if algo == "POLYMUL_CHAIN":
            algo_chain = PolymulAlgo(algo, 2, ["KARATSUBA", "TEXTBOOK"])
            algo_chain.build()
            pq_testing.test_m4_pq(algo_chain, key_num, text_num)


if __name__ == "__main__":
    logging.basicConfig(format='%(message)s', level=logging.INFO)
    main()
