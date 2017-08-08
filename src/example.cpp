#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

static void gpio_setup() {
    rcc_clock_setup_in_hse_8mhz_out_72mhz();
    rcc_periph_clock_enable(RCC_GPIOB);
    gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO14);
}

void example_start() {
    int i;
    gpio_setup();
    for (;;) {
        gpio_toggle(GPIOB, GPIO14);
        for (i = 0; i < 800000; i++) {
            __asm__("nop");
        }
    }
}

