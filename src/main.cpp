#include <libopencm3/stm32/gpio.h>

int main() {
    /* Taken from the docs */
    gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ,
                  GPIO_CNF_OUTPUT_PUSHPULL,  GPIO2 | GPIO9);
    gpio_set(GPIOC, GPIO2 | GPIO9);
    gpio_clear(GPIOC, GPIO2);
    gpio_toggle(GPIOC, GPIO2 | GPIO9);
    gpio_port_write(GPIOC, 0x204);
    /* :) */
    return 0;
}
