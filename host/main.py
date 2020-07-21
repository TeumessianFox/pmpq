import pq_testing
import logging
import evaluation


def main():
    evaluation.textbook_eval()


def test_algos():
    seed = 168
    logging.info("Seed: {}".format(seed))
    key_num = pq_testing.key_gen(seed)
    text_num = pq_testing.text_gen(seed)
    for algo in pq_testing.POLYMUL_ALGOS:
        if algo == "POLYMUL_CHAIN":
            pq_testing.init(algo, 3, ["TOOM-COOK-3", "TEXTBOOK"])
            output = pq_testing.test_m4_pq(algo, key_num[0:6], text_num[0:6])


if __name__ == "__main__":
    logging.basicConfig(format='%(message)s', level=logging.INFO)
    main()
