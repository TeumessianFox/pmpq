import m4serial
import pq_testing


def main():
    m4serial.init()
    pq_testing.test_m4_pq(666)


if __name__ == "__main__":
    main()
