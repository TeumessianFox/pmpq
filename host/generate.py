from numpy.random import RandomState
from numpy import uint16
import numpy as np

# Streamlined NTRU Prime: sntrup4591761
P, Q, W = 761, 4591, 286


def key_gen_sntrup4591761(seed):
    r = RandomState(seed)
    c_values = 2 * r.randint(2, size=W) - 1  # W "small" non-zero coefficients, i.e. -1, 1
    c_pos = r.choice(P, W, replace=False)  # choose W positions out of the P possible ones
    cs = np.zeros(P, dtype=uint16)
    for i in range(W):  # fill the W non-zero values
        cs[c_pos[i]] = c_values[i]
    return cs


def text_gen_sntrup4591761(seed):
    return (np.random.default_rng(seed=seed).integers(0, Q, size=P) // 3) * 3


def random_gen(seed, degree=1024, max_coefficient=Q) -> ([], []):
    return np.split(np.random.default_rng(seed=seed).integers(0, max_coefficient, size=2*degree), 2)
