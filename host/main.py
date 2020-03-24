import m4serial
import pq_testing


def main():
    m4serial.init()

    seed = 666
    key_num = pq_testing.key_gen(seed)
    text_num = pq_testing.text_gen(seed)
    output = pq_testing.test_m4_pq(key_num, text_num)


if __name__ == "__main__":
    main()
