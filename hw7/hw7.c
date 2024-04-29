/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


#include <stdio.h>
#include <string.h>
#include "pico/binary_info.h"
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "ssd1306.h"
#include "font.h"
#include "hardware/uart.h"
#include "hardware/irq.h"

#define LED_PIN 25


/// \tag::uart_advanced[]

#define UART_ID uart0
#define BAUD_RATE 115200
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY    UART_PARITY_NONE

// We are using pins 0 and 1, but see the GPIO function select table in the
// datasheet for information on which other pins can be used.
#define UART_TX_PIN 0
#define UART_RX_PIN 1

static int chars_rxed = 0;

void drawChar(int x,  int y, char letter) {
    int i, j;
    for (i=0; i<5; i++) {
        char c = ASCII[letter - 32][i];
        for (j=0; j<8; j++) {
            char bit = (c >> j) & 0b1; //take out only the bit we're interested in

            if (bit == 0b1) {
                ssd1306_drawPixel(x+i, y+j, 1);
            }
            else {
                ssd1306_drawPixel(x+i,y+j, 0);
            }
        }
    }
}

void drawMessage(int x, int y, char * m) {
    int i = 0;
    while (m[i]) { //until it reaches null character
        drawChar(x+5*i, y, m[i]);
        i++;
    }

    ssd1306_update();

}

volatile int i = 0;
volatile char m[100];

// RX interrupt handler
void on_uart_rx() {
    while (uart_is_readable(UART_ID)) {
        uint8_t ch = uart_getc(UART_ID);
    if (ch == '\r') {
        ssd1306_clear();
        ssd1306_update();
        drawMessage(0, 0, m);
        m[0] = 0;
        i = 0;
    }
    else {
        m[i] = ch;
        i++;
    }
        chars_rxed++;
    }
}

int main() {
    // Set up our UART with a basic baud rate.
    uart_init(UART_ID, 115200);

    // Set the TX and RX pins by using the function select on the GPIO
    // Set datasheet for more information on function select
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    // Set UART flow control CTS/RTS, we don't want these, so turn them off
    uart_set_hw_flow(UART_ID, false, false);

    // Set our data format
    uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);

    // Turn off FIFO's - we want to do this character by character
    uart_set_fifo_enabled(UART_ID, false);

    // Set up a RX interrupt
    // We need to set up the handler first
    // Select correct interrupt for the UART we are using
    int UART_IRQ = UART_ID == uart0 ? UART0_IRQ : UART1_IRQ;

    // And set up and enable the interrupt handlers
    irq_set_exclusive_handler(UART_IRQ, on_uart_rx);
    irq_set_enabled(UART_IRQ, true);

    // Now enable the UART to send interrupts - RX only
    uart_set_irq_enables(UART_ID, true, false);



    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    i2c_init(i2c_default, 400*1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);

    ssd1306_setup();

    sleep_ms(100);

    // OK, all set up.
    // Lets send a basic string out, and then run a loop and wait for RX interrupts
    // The handler will count them, but also reflect the incoming data back with a slight change!
    uart_puts(UART_ID, "\nHello, uart interrupts\n");

    while (1) {
        gpio_put(LED_PIN, 1);
        sleep_ms(25);
        gpio_put(LED_PIN, 0);
        sleep_ms(25);
    }
       
}

/// \end:uart_advanced[]
