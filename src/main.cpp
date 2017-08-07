#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

int main() {
    rcc_periph_clock_enable(RCC_GPIOB);
    gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_2_MHZ,
                  GPIO_CNF_OUTPUT_PUSHPULL,  GPIO12 | GPIO13 | GPIO14);
    int i;
    for(;;) {
        gpio_toggle(GPIOB, GPIO12 | GPIO13 | GPIO14);
        for (i = 0; i < 800000; i++)	/* Wait a bit. */
                __asm__("nop");
    }
    return 0;
}
