import logging
from evaluation import Evaluation


def main():
    eval = Evaluation()
    eval.evaluate_all()


if __name__ == "__main__":
    logging.basicConfig(format='%(message)s', level=logging.INFO)
    main()
