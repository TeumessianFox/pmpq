#include "hal-stm32f4.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/rng.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/flash.h>
#include <libopencm3/cm3/systick.h>


/* 24 MHz */
const struct rcc_clock_scale benchmarkclock = {
  .pllm = 8, //VCOin = HSE / PLLM = 1 MHz
  .plln = 192, //VCOout = VCOin * PLLN = 192 MHz
  .pllp = 8, //PLLCLK = VCOout / PLLP = 24 MHz (low to have 0WS)
  .pllq = 4, //PLL48CLK = VCOout / PLLQ = 48 MHz (required for USB, RNG)
  .pllr = 0,
  .hpre = RCC_CFGR_HPRE_DIV_NONE,
  .ppre1 = RCC_CFGR_PPRE_DIV_2,
  .ppre2 = RCC_CFGR_PPRE_DIV_NONE,
  .pll_source = RCC_CFGR_PLLSRC_HSE_CLK,
  .voltage_scale = PWR_SCALE1,
  .flash_config = FLASH_ACR_DCEN | FLASH_ACR_ICEN | FLASH_ACR_LATENCY_0WS,
  .ahb_frequency = 24000000,
  .apb1_frequency = 12000000,
  .apb2_frequency = 24000000,
};

static void clock_setup(const enum clock_mode clock)
{
  switch(clock)
  {
    case CLOCK_BENCHMARK:
      rcc_clock_setup_pll(&benchmarkclock);
      break;
    case CLOCK_FAST:
    default:
      rcc_clock_setup_pll(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_168MHZ]);
      break;
  }

  rcc_periph_clock_enable(RCC_GPIOA);
  rcc_periph_clock_enable(RCC_USART1);
  rcc_periph_clock_enable(RCC_DMA1);
  rcc_periph_clock_enable(RCC_RNG);

  flash_prefetch_enable();
}

static void gpio_setup(void)
{
  gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO9 | GPIO10);
  gpio_set_output_options(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO9 | GPIO10);
  gpio_set_af(GPIOA, GPIO_AF7, GPIO9 | GPIO10);
}

static void usart_setup(int baud)
{
  usart_set_baudrate(USART1, baud);
  usart_set_databits(USART1, 8);
  usart_set_stopbits(USART1, USART_STOPBITS_1);
  usart_set_mode(USART1, USART_MODE_TX_RX);
  usart_set_parity(USART1, USART_PARITY_NONE);
  usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);

  usart_enable(USART1);
}

static void systick_setup(void)
{
  systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
  systick_set_reload(16777215);
  systick_interrupt_enable();
  systick_counter_enable();
}

static int send_USART_str(const char* in)
{
  int i;
  for(i = 0; in[i] != 0; i++) {
    usart_send_blocking(USART1, *(unsigned char *)(in+i));
  }
  return i;
}
void hal_setup(const enum clock_mode clock)
{
  (void)(clock);
  clock_setup(CLOCK_FAST);
  gpio_setup();
  usart_setup(38400);
  systick_setup();
  rng_enable();
}

void hal_send_char(char c)
{
  usart_send_blocking(USART1, c);
}

int hal_send_str(const char* in)
{
  return send_USART_str(in);
}

uint16_t hal_recv()
{
  return usart_recv_blocking(USART1);
}

static volatile unsigned long long overflowcnt = 0;
void sys_tick_handler(void)
{
  ++overflowcnt;
}
uint64_t hal_get_time()
{
  while (true) {
    unsigned long long before = overflowcnt;
    unsigned long long result = (before + 1) * 16777216llu - systick_get_value();
    if (overflowcnt == before) {
      return result;
    }
  }
}
