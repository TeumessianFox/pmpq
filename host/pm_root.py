import numpy as np
import matplotlib.pyplot as plt
import os.path
import scipy.stats as stats

MIN_LEN = 128
MAX_LEN = 1024
SEQ_LEN = 10


def create_numbers():
    # All numbers to check
    sizes = np.zeros(MAX_LEN - MIN_LEN + 1)
    for i in range(MAX_LEN - MIN_LEN + 1):
        sizes[i] = MIN_LEN + i
    return sizes


def create_sequences():
    # All possible sequences of two's and three's
    filename = "base/sequences.npy"
    if os.path.isfile(filename):
        sequences = np.load(filename)
        print("Loading {}".format(filename))
    else:
        sequences = np.zeros((SEQ_LEN ** 3, SEQ_LEN), dtype=int)
        for index in range(SEQ_LEN ** 3):
            current = index
            for three in range(SEQ_LEN):
                if current % 2 == 0:
                    sequences[index][three] = 2
                else:
                    sequences[index][three] = 3
                current = current // 2
        np.save(filename, sequences)
    return sequences


def create_divider(numbers, sequences):
    # Apply all sequences to all numbers to find bases
    filename = "base/divider.npy"
    if os.path.isfile(filename):
        divider = np.load(filename)
        print("Loading {}".format(filename))
    else:
        divider = np.zeros((len(numbers), SEQ_LEN ** 3, SEQ_LEN), dtype=int)
        for i in range(len(numbers)):
            for x in range(SEQ_LEN ** 3):
                current = numbers[i]
                for y in range(SEQ_LEN):
                    divider[i][x][y] = current
                    next: int = current // sequences[x][y]
                    if next == 1 or next == 2:
                        break
                    if current % sequences[x][y] != 0:
                        next += current % sequences[x][y]
                    current = next
        np.save(filename, divider)
    return divider


def count_base_for_numbers(numbers, divider):
    # Find bases for each number
    filename = "base/counter.npy"
    if os.path.isfile(filename):
        counter = np.load(filename)
        print("Loading {}".format(filename))
    else:
        counter = np.zeros((len(numbers), MAX_LEN + 1), dtype=int)
        for i in range(len(numbers)):
            for x in range(SEQ_LEN ** 3):
                for y in range(SEQ_LEN):
                    counter[i][divider[i][x][y]] += 1
        np.save(filename, counter)
    return counter


def find_common_bases(numbers, counter):
    # Find common bases among numbers
    common_counter = np.zeros(MAX_LEN + 1, dtype=int)
    for i in range(len(numbers)):
        for c in range(MAX_LEN + 1):
            if counter[i][c] != 0:
                common_counter[c] += 1
    return common_counter


def find_loss_mean_std(numbers, counter):
    # Reduce to possible bases for each number
    nbases = (counter > 0)
    losses = np.zeros((13, 25), dtype=int)
    for base in range(12, 25):
        for i in range(len(numbers)):
            current_bool = nbases[i][base]
            lose_factor = 0
            while not current_bool:
                lose_factor += 1
                current_bool = nbases[i][base - lose_factor]
            losses[base - 12][base - lose_factor] += 1
    mean = np.zeros(13, dtype=float)
    std = np.zeros(13, dtype=float)
    for base in range(12, 25):
        for i in range(25):
            current = losses[base - 12][i]
            for value in range(current):
                mean[base - 12] += i
        mean[base - 12] = mean[base - 12] / len(numbers)
    for base in range(12, 25):
        for i in range(25):
            current = losses[base - 12][i]
            for value in range(current):
                std[base - 12] += (i - mean[base - 12]) ** 2
        std[base - 12] = np.sqrt(std[base - 12] / len(numbers))
    return losses, mean, std


def main():
    plt.close('all')

    numbers = create_numbers()
    sequences = create_sequences()
    divider = create_divider(numbers, sequences)
    counter = count_base_for_numbers(numbers, divider)
    common_counter = find_common_bases(numbers, counter)
    losses, mean, std = find_loss_mean_std(numbers, counter)

    for base in range(13):
        loss = np.linspace(0, 25, 25)
        plt.figure(base)
        plt.bar(loss, losses[base])
        plt.title("Using Schoolbook_{}x{}".format(base+12, base+12))
        plt.xlabel("Closest to Schoolbook_{}x{}".format(base+12, base+12))
        plt.ylabel("#Numbers for this base")
        plt.xlim(0, 26)
        plt.ylim(0, losses[base][base+12] + 100)
        plt.text(3, losses[base][base+12], r'$\mu={:.4f}$'.format(mean[base]))
        plt.text(3, losses[base][base+12] - 50, r'$\sigma={:.4f}$'.format(std[base]))
        plt.tight_layout(pad=0.4, w_pad=0.5, h_pad=1.0)
        plt.savefig("base/schoolbook_{}x{}.pdf".format(base+12, base+12), bbox_inches='tight')


if __name__ == "__main__":
    main()
