/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/spi.h"

#define M_PI acos(-1.0)


static void write_register(uint8_t channel, uint16_t data) {
    uint8_t buf[2];
    buf[0] = (channel << 7) | 0b01110000 | (data >> 6);  
    buf[1] = (data << 2);
    gpio_put(17, 0);
    spi_write_blocking(spi_default, buf, 2);
    gpio_put(17, 1);
    sleep_ms(10);
}



int main() {
    stdio_init_all();
    // This example will use SPI0 at 0.5MHz.
    spi_init(spi_default, 500 * 1000);
    gpio_set_function(PICO_DEFAULT_SPI_RX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI);
    // Make the SPI pins available to picotool
    
    // Chip select is active-low, so we'll initialise it to a driven-high state
    gpio_init(PICO_DEFAULT_SPI_CSN_PIN);
    gpio_set_dir(PICO_DEFAULT_SPI_CSN_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 1);
    // Make the CS pin available to picotool

    while (true) {
        // write_register(0, 0b0000000000); //writing to channel a
        for (int i=0; i<50; i++) {
            uint16_t data = (double) i / 50 * 1023;
            write_register(0, data);
        }
        for (int i=50; i >0 ; i--) {
            uint16_t data = (double) i /50 * 1023;
            write_register(0, data);
        }
        //write_register(1, 0b0000000000); //writing to channel b
        for (int i=0; i<= 100; i++) {
            double fr = 1.0;
            double value = sin(2 * M_PI * fr * i / 50);   
            uint16_t data = (uint16_t)((value + 1) * 511.5); 
            write_register(1, data);
        }
    }

}
