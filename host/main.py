import logging
from evaluation import Evaluation
import ntt_test


def main():
    #eval = Evaluation()
    #eval.evaluate_all()
    ntt_test.test()


if __name__ == "__main__":
    logging.basicConfig(format='%(message)s', level=logging.INFO)
    main()
