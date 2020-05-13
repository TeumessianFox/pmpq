import pq_testing
import logging


def main():
    logging.basicConfig(format='%(message)s', level=logging.INFO)
    seed = 666
    key_num = pq_testing.key_gen(seed)
    text_num = pq_testing.text_gen(seed)
    for algo in pq_testing.POLYMUL_ALGOS:
        if algo == "POLYMUL_CHAIN":
            pq_testing.init(algo, 3, ["TOOM-COOK-3", "ASM_SCHOOLBOOK_24"])
            output = pq_testing.test_m4_pq(algo, key_num[0:72], text_num[0:72])


if __name__ == "__main__":
    main()
