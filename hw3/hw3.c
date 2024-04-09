/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pico/stdlib.h"
#include "hardware/adc.h"
#include <stdio.h>


int main() {
#ifndef PICO_DEFAULT_LED_PIN
#warning blink example requires a board with a regular LED
#else
    const uint BUTTON = 18;
    const uint LED_PIN = 16;
    gpio_init(BUTTON);
    gpio_init(LED_PIN);
    gpio_set_dir(BUTTON, GPIO_IN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    adc_init(); // init the adc module
    adc_gpio_init(28); // set ADC0 pin to be adc input instead of GPIO
    adc_select_input(2); // select to read from ADC0

    stdio_init_all();
    while (!stdio_usb_connected()) {
        sleep_ms(100);
    }
    printf("Start!\n");

    while (true) {
        gpio_put(LED_PIN, 1);
        while (gpio_get(BUTTON) != 1) { //waiting for button to be pressed. if button is 0, it is continuously stuck on loop.
        }
        gpio_put(LED_PIN, 0);
        int analogSamples;
        scanf("%d", &analogSamples);
        for (int i = 0; i < analogSamples; i++) {
            uint16_t result = adc_read();
            float voltage = result * 3.3 / 4095;
            printf("%f\n", voltage);
        }
    }
#endif
}
