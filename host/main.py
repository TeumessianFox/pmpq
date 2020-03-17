import m4serial


def main():
    m4serial.init()
    m4serial.simpleserial_put('k', [1, 2, 3, 4])
    result = m4serial.simpleserial_get('r')
    print(result)
    m4serial.simpleserial_put('p', [4, 3, 2, 1])
    result = m4serial.simpleserial_get('r')
    print(result)
    result = m4serial.simpleserial_get('r')
    print(result)


if __name__ == "__main__":
    main()
