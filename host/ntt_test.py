from numpy.random import RandomState
from numpy import uint16
import numpy as np
from pm_algo import PolymulAlgo
import logging

# Kyber 512
Q = 3329
DEGREE = 256


def key_gen_kyber512(seed):
    """
    (https://pq-crystals.org/kyber/data/kyber-specification.pdf)
    Kyber is a learning-with-errors problem in module lattice
    Kyber512 with n= 256, k=2, q=3329 and eta(aka. n) = 5
    Kyber uses NTT instead of Karatsuba or Toom-Cook due to it's ring modulo
    Kyber uses polynomial multiplication to multiply matrix A with vector s
    Matrix A is of size kxk where each element is a 32 byte vector in R_q
    Vector s is of size k where each element is a 32 byte vector in R_q
    Matrix A is random uniformed in R_q with all elements < q (aka.
    random_poly() could be used with a specific high value of 7681
    This method will thus create a distribution close to the distribution of
    the actual noise used in Kyber
    s is a centered binomial distribution of a pseudorandom distribution
    """
    eta = 2
    rng = np.random.default_rng(seed=seed)  # pseudorandom (normally SHAKE-256)
    p = 0.5  # distribution
    s = rng.binomial(eta, p, 2 * DEGREE)  # binomial distribution
    s = s.astype(np.uint16)
    poly = s[0:DEGREE] - s[DEGREE: 2 * DEGREE]  # centered
    if Q is not None:
        poly %= Q
    poly = poly.astype(int)
    return poly


def text_gen_kyber512(seed):
    return np.random.default_rng(seed=seed).integers(0, Q, size=DEGREE)


def test():
    seed = np.random.default_rng().integers(0, 1000, size=1)[0]
    logging.info("Seeds: {}".format(seed))
    key = key_gen_kyber512(seed)
    text = text_gen_kyber512(seed)
    algo_ntt = PolymulAlgo("NTT")
    algo_ntt.build()
    cycle = algo_ntt.run_polymul(key, text)[1]
