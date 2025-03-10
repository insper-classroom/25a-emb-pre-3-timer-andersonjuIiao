#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

const int BTN_PIN_R = 28;
const int BTN_PIN_G = 26;

const int LED_PIN_R = 4;
const int LED_PIN_G = 6;

volatile int flag_r = 0;
volatile int flag_g = 0;

volatile int g_timer_red = 0;
volatile int g_timer_green = 0;

void btn_callback(uint gpio, uint32_t events) {
    if (events == 0x4) {
        if (gpio == BTN_PIN_R)
            flag_r = !flag_r;
        else if (gpio == BTN_PIN_G)
            flag_g = !flag_g;
    }
}

bool timer_red_callback(repeating_timer_t *rt) {
    g_timer_red = 1;
    return true; // keep repeating
}

bool timer_green_callback(repeating_timer_t *rt) {
    g_timer_green = 1;
    return true; // keep repeating
}

int main() {
    gpio_init(LED_PIN_R);
    gpio_set_dir(LED_PIN_R, GPIO_OUT);

    gpio_init(LED_PIN_G);
    gpio_set_dir(LED_PIN_G, GPIO_OUT);

    gpio_init(BTN_PIN_R);
    gpio_set_dir(BTN_PIN_R, GPIO_IN);
    gpio_pull_up(BTN_PIN_R);

    gpio_init(BTN_PIN_G);
    gpio_set_dir(BTN_PIN_G, GPIO_IN);
    gpio_pull_up(BTN_PIN_G);

    gpio_set_irq_enabled_with_callback(BTN_PIN_R, GPIO_IRQ_EDGE_FALL, true,
                                       &btn_callback);
    gpio_set_irq_enabled(BTN_PIN_G, GPIO_IRQ_EDGE_FALL, true);

    repeating_timer_t timer_red;

    if (!add_repeating_timer_ms(500,
        timer_red_callback,
        NULL,
        &timer_red))
    {
    printf("Failed to add timer RED\n");
    }

    repeating_timer_t timer_green;

    if (!add_repeating_timer_ms(250,
        timer_green_callback,
        NULL,
        &timer_green))
    {
    printf("Failed to add timer Green\n");
    }

    while (true) {

        if (flag_r) {

            if(g_timer_red){
                gpio_put(LED_PIN_R,!gpio_get(LED_PIN_R));
                g_timer_red = 0;
            }
        }
        else if(!flag_r){
            gpio_put(LED_PIN_R,0);
        }

        if (flag_g) {

            if(g_timer_green){
                gpio_put(LED_PIN_G,!gpio_get(LED_PIN_G));
                g_timer_green = 0;
            }
        }
        else if(!flag_g){
            gpio_put(LED_PIN_G,0);
        }
    
    }
}
