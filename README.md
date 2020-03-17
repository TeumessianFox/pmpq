# Polynomial multiplication for post-quantum algorithms
Based on https://github.com/libopencm3/libopencm3-template.git

## Install
 1. ```git clone https://github.com/TeumessianFox/pmpq.git```
 2. ```cd pmpq```

### arm-none-eabi toolchain:
 On Linux systems:
 
 3. ```git apt install arm-none-eabi-gcc```

### Libopencm3:
 4. ```git submodule update --init```
 5. ```make -C libopencm3```

### stlink:
 6. Install [st-link](https://github.com/texane/stlink.git)

## Usage

### Python
*Python >= 3.5  needed*
#### Run
To try the latest version run `host/main.py`

#### M4Serial
| Function | Parameter | Description |
| --- | --- | --- |
| `init()` | *void* | Flashing latest code to the M4 and setting up serial communication |
| `simpleserial_get(...)` | *char* c | Get a payload with the leading command c |
| `simpleserial_put(...)` | *char* c, *int array* data | Send command c with payload data |

### Makefile
*Recommended*: Use python `pypmpq.m4serial.init()` to make & flash
| Command | Description |
| --- | --- |
| `make` | Compile and create .elf &  .bin |
| `make flash` | Flash .bin on the board |
| `make dump` | Using OBJDUMP to create dump |
| `make clean` | Remove created files |


## Coding style

**C language:** [kernel coding style](https://www.kernel.org/doc/Documentation/process/coding-style.rst)

**Python:** [PEP8](https://www.python.org/dev/peps/pep-0008/) using [Flake8](https://flake8.pycqa.org/en/latest/)

## Authors

[Patrick Gersch](https://github.com/teumessianfox/)
