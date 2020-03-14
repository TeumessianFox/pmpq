# Polynomial multiplication for post-quantum algorithms
Based on https://github.com/libopencm3/libopencm3-template.git

## Install
 1. ```git clone https://github.com/TeumessianFox/pmpq.git```
 2. ```cd pmpq```

### arm-none-eabi toolchain:
 On Linux systems:
 
 3. ```git apt install arm-none-eabi-gcc```

### Libopencm3:
 4. ```git submodule update --init #```
 5. ```make -C libopencm3 #```

### stlink:
 6. Install [st-link](https://github.com/texane/stlink.git)

## Usage
### Run
```
cd polymul
make
```

### Flash
```
make flash
```

## Coding style

Please follow the [coding style](https://www.kernel.org/doc/Documentation/process/coding-style.rst) for C.

## Authors

[Patrick Gersch](https://github.com/teumessianfox/)
