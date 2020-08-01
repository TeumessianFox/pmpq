# Efficient Implementation of Polynomial Multiplication in Post Quantum Cryptography


## Install
 1. ```git clone https://github.com/TeumessianFox/pmpq.git```
 2. ```cd pmpq```

### arm-none-eabi toolchain:
 On Linux systems:
 
 3. ```sudo apt install arm-none-eabi-gcc```

### Libopencm3:
 4. ```git submodule update --init```
 5. ```make -C libopencm3```

### stlink:
 6. Install [st-link](https://github.com/texane/stlink.git)
 
### serial:
 7. ```python3 -m pip3 install pyserial```

## Usage

The main objective of this library is to provide different implementations of polynomial multiplication for post-quantum crypto algorithms.

### Target
The **STM32F407G-DISCOVERY** is used for the [pqm4](https://github.com/mupq/pqm4) contest and also the main target for this library. 

Flashable .hex files can also be generated for the **Chipwhisperer**.

### Python
*Python >= 3.7  needed*
#### Run
Run `host/main.py` to try every supported polynomial multiplication on the STM32F407G-DISCOVERY once and gather results & cycle count.

#### pm_algo
Access `pm_algo.POLYMUL_ALGOS` to gather all viable **algo_names**

PolymulAlgo class
| Function | Parameter | Description |
| --- | --- | --- |
| `init(...)` | self, name: *str*, chain_size: *int*, chain: *str array*, degree: *int*, opt: *char* | Flashing **algo** code to the M4 and setting up serial communication |
| `build(...)` | self | Make and flashing **PolymulAlgo** code to the M4 |
| `log_to_file(...)` | self, key_num, text_num, result, cycles | Log results to file |
| `run_polymul(...)` | self, key, text, log: *boolean* | Run specific **PolymulAlgo** for specific testset |

### Makefile

#### STM32F407G-DISCOVERY
*Recommended*: Use python `PolymulAlgo.build()` to make & flash

For manual use in `m4/`
| Command | Description |
| --- | --- |
| `make` | Compile and create .elf & .bin|
| `make flash` | Flash .bin on the board |
| `make dump` | Using OBJDUMP to create dump |
| `make clean` | Remove created files |

#### Chipwhisperer
To generate .hex file for cw use in `cw/`
| Command | Description |
| --- | --- |
| `make` | Compile and create .elf & .hex|
| `make clean` | Remove created files |

## Coding style

**C language:** [coding style](https://www.kernel.org/doc/Documentation/process/coding-style.rst)

**Python:** [PEP8](https://www.python.org/dev/peps/pep-0008/) using [Flake8](https://flake8.pycqa.org/en/latest/)

## Authors

[Patrick Gersch](https://github.com/teumessianfox/)

## Credits

Based on https://github.com/libopencm3/libopencm3-template.git
