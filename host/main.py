import pq_testing
import logging


def main():
    logging.basicConfig(format='%(message)s', level=logging.INFO)
    for algo in pq_testing.POLYMUL_ALGOS:
        pq_testing.init(algo)
        seed = 666
        key_num = pq_testing.key_gen(seed)
        text_num = pq_testing.text_gen(seed)
        output = pq_testing.test_m4_pq(algo, key_num, text_num)


if __name__ == "__main__":
    main()
