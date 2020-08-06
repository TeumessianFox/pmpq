import numpy as np
import os.path
import logging

MIN_LEN = 64
MAX_LEN = 1024
SEQ_LEN = 5
SCHOOLBOOKS = [12, 16, 24]
SCHOOLBOOK_NAMES = ["ASM_SCHOOLBOOK_12", "ASM_SCHOOLBOOK_16", "ASM_SCHOOLBOOK_24"]


def create_sequences():
    # All possible sequences of Karatsuba (split by 2) and Toom-Cook-3 (split by 3)
    filename = "base/sequences.npy"
    if os.path.isfile(filename):
        sequences = np.load(filename)
        logging.info("Loading {}".format(filename))
    else:
        sequences = []
        last_layer = [[1, 0, 0, 0, 0]]
        for seq in range(0, SEQ_LEN - 1):
            current_layer = []
            for layer in last_layer:
                if layer[seq] == 1 or layer[seq] == 2:
                    two_layer = layer.copy()
                    two_layer[seq + 1] = 2
                    current_layer.append(two_layer)
                three_layer = layer.copy()
                three_layer[seq + 1] = 3
                current_layer.append(three_layer)
            last_layer = current_layer
            sequences.extend(current_layer.copy())
        sequences = np.array(sequences)
        np.save(filename, sequences)
    return sequences


def create_schoolbook_sequences(schoolbooks, sequences):
    filename = "base/schoolbook_sequences.npy"
    if os.path.isfile(filename):
        sseq = np.load(filename)
        logging.info("Loading {}".format(filename))
    else:
        sseq = np.zeros((len(schoolbooks), len(sequences), SEQ_LEN), dtype=int)
        for sb, schoolbook in enumerate(schoolbooks):
            for sq, seq in enumerate(sequences):
                for i in range(SEQ_LEN):
                    if seq[i] == 1:
                        sseq[sb][sq][i] = schoolbook
                    else:
                        if sseq[sb][sq][i - 1] * seq[i] < 1025:
                            sseq[sb][sq][i] = sseq[sb][sq][i - 1] * seq[i]
        np.save(filename, sseq)
    return sseq


def create_sorted_set(schoolbook_sequences):
    sorted_set = []
    for schoolbook in schoolbook_sequences:
        flat_list = [item for sublist in schoolbook for item in sublist]
        sorted_set.append(sorted(set(flat_list)))
    for schoolbook in sorted_set:
        schoolbook.remove(0)
    return sorted_set


def sequences_to_polymul_chains(schoolbook_names, sequences):
    filename = "base/schoolbook_chains.npy"
    if os.path.isfile(filename):
        schoolbooks_chains = np.load(filename, allow_pickle=True)
        logging.info("Loading {}".format(filename))
    else:
        schoolbooks_chains = []
        for s, schoolbook in enumerate(schoolbook_names):
            schoolbook_chains = []
            for seq in sequences:
                layer_chain = []
                degree = 0
                for element in seq:
                    if element == 1:
                        layer_chain.append(schoolbook)
                        degree = SCHOOLBOOKS[s]
                    elif element == 2:
                        layer_chain.append("KARATSUBA")
                        degree *= 2
                    elif element == 3:
                        layer_chain.append("TOOM-COOK-3")
                        degree *= 3
                    else:
                        break
                if degree < 1024:
                    layer_chain = layer_chain[::-1]
                    schoolbook_chains.append(layer_chain)
            schoolbooks_chains.append(schoolbook_chains)
        np.save(filename, schoolbooks_chains, allow_pickle=True)
    return schoolbooks_chains


def polymul_chains():
    sequences = create_sequences()
    schoolbook_sequences = create_schoolbook_sequences(SCHOOLBOOKS, sequences)
    sorted_set = create_sorted_set(schoolbook_sequences)
    schoolbooks_chains = sequences_to_polymul_chains(SCHOOLBOOK_NAMES, sequences)
    logging.debug(sorted_set)
    logging.debug(schoolbooks_chains)
    logging.debug(schoolbook_sequences)
    return schoolbook_sequences, schoolbooks_chains
