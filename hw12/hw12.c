#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/pwm.h"

 #define LEDPin 3 // the built in LED on the Pico

void set_degrees(double num) {
    double input = (num / 180) * 0.1 + 0.025 ;
    pwm_set_gpio_level(LEDPin, 62500 * input);
}

void pwm_cycle_init() {
    gpio_set_function(LEDPin, GPIO_FUNC_PWM); // Set the LED Pin to be PWM
    uint slice_num = pwm_gpio_to_slice_num(LEDPin); // Get PWM slice number
    float div = 40; // must be between 1-255
    pwm_set_clkdiv(slice_num, div); // divider
    uint16_t wrap = 62500; // when to rollover, must be less than 65535
    pwm_set_wrap(slice_num, wrap);
    pwm_set_enabled(slice_num, true); // turn on the PWM
    //pwm_set_gpio_level(LEDPin, wrap / 10); // set the duty cycle to 50%
}


int main (){
    stdio_init_all();
    pwm_cycle_init();
    while (true) {
        for (int i = 0; i < 100; i++) {
            double angle = (double) i / 99.0 * 180.0;
            set_degrees(angle);
            sleep_ms(20);
        }
        for (int i = 99; i >= 0; i--) {
            double angle = (double) i / 99.0 * 180.0;
            set_degrees(angle);
            sleep_ms(20);
        }
    }
}