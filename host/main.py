import pq_testing
import logging


def main():
    logging.basicConfig(format='%(message)s', level=logging.INFO)
    seed = 666
    key_num = pq_testing.key_gen(seed)
    text_num = pq_testing.text_gen(seed)
    for algo in pq_testing.POLYMUL_ALGOS:
        pq_testing.init(algo)
        output = pq_testing.test_m4_pq(algo, key_num[0:23], text_num[0:23])


if __name__ == "__main__":
    main()
