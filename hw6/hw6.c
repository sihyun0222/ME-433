#include <stdio.h>
#include <string.h>
#include "pico/binary_info.h"
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "ssd1306.h"
#include "font.h"

#define LED_PIN 25

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

int main() {

    stdio_init_all();

    adc_init(); // init the adc module
    adc_gpio_init(26); // set ADC0 pin to be adc input instead of GPIO
    adc_select_input(0); // select to read from ADC0

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    i2c_init(i2c_default, 400*1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);

    ssd1306_setup();

    sleep_ms(100);

    uint16_t result;
    double voltage;
    char message[50];

    while (true) {
        gpio_put(LED_PIN, 1);
        sleep_ms(25);
        gpio_put(LED_PIN, 0);
        sleep_ms(25);
        
        unsigned int start = to_us_since_boot(get_absolute_time());

        result = adc_read();
        voltage = result * 3.3 / 4095;
        sprintf(message,"Voltage: %f", voltage);
        drawMessage(1, 1, message);

        unsigned int stop = to_us_since_boot(get_absolute_time());

        unsigned int t = stop - start;
        sprintf(message, "FPS = %f", 1000000.0/t);
        drawMessage(1, 24, message);
        ssd1306_update();
        sleep_ms(250);



    }

    
}