import pq_testing
import logging
import evaluation
from pm_algo import PolymulAlgo, POLYMUL_ALGOS


def main():
    evaluation.schoolbook_eval()
    evaluation.toom_3_eval()
    evaluation.textbook_eval()
    evaluation.karatsuba_eval()


if __name__ == "__main__":
    logging.basicConfig(format='%(message)s', level=logging.INFO)
    main()
