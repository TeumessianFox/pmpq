# Polynomial multiplication for post-quantum algorithms
Based on https://github.com/libopencm3/libopencm3-template.git

# Install
```
 1. git clone https://github.com/TeumessianFox/pmpq.git
 2. cd pmpq
```
OpenOCD:
```
 3. git apt install openocd
```
Libopencm3:
```
 4. git submodule update --init # (Only needed once)
 5. make -C libopencm3 # (Only needed once)
```

Run:
```
 5. cd polymul
 6. make
```

Flash:
```
 7. make flash
```

## Coding style

Please follow the [coding style](https://www.kernel.org/doc/Documentation/process/coding-style.rst) for C.

## Authors

[Patrick Gersch](https://github.com/teumessianfox/)
