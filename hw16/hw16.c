#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/pwm.h"


#define MOTOR_IN4 20   
#define MOTOR_IN3 19  

#define PWM_RANGE 62500 


void set_motor_speed(int speed) {
    if (speed >= 0) {
        // Forward direction
        pwm_set_gpio_level(MOTOR_IN4, PWM_RANGE * speed / 20);
        pwm_set_gpio_level(MOTOR_IN3, 0);
    } else {
        // Reverse direction
        pwm_set_gpio_level(MOTOR_IN4, 0);
        pwm_set_gpio_level(MOTOR_IN3, PWM_RANGE * (-speed) / 20);
    }
}


void pwm_pin_init() {
    gpio_set_function(MOTOR_IN4, GPIO_FUNC_PWM);
    gpio_set_function(MOTOR_IN3, GPIO_FUNC_PWM);

    uint slice_num_4 = pwm_gpio_to_slice_num(MOTOR_IN4);
    uint slice_num_3 = pwm_gpio_to_slice_num(MOTOR_IN3); 

    float div = 40; 
    pwm_set_clkdiv(slice_num_4, div);
    pwm_set_clkdiv(slice_num_3, div);

    pwm_set_wrap(slice_num_4, PWM_RANGE); 
    pwm_set_wrap(slice_num_3, PWM_RANGE); 

    pwm_set_enabled(slice_num_4, true); 
    pwm_set_enabled(slice_num_3, true); 
}

int main() {
    stdio_init_all(); 
    pwm_pin_init();

while (true) {
    int speed = 0;
    scanf("%d", &speed); // Read input directly from standard input
    if (speed < -20 || speed > 20) {
        speed = 0;
    }

    // Set motor speed if it's not 0
    if (speed != 0) {
        set_motor_speed(speed); // Set motor speed
    } else {
        // If speed is 0, stop the motor
        pwm_set_gpio_level(MOTOR_IN4, 0);
        pwm_set_gpio_level(MOTOR_IN3, 0);
    }
}

}

