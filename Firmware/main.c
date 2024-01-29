#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/time.h"
#include "hardware/pwm.h"

#include "CC_Charger.h"
#include "pulse_generator.h"

//Pinout definitions
#define CUT_nEN_PIN 0

//OUTPUT PULSE PARAMETERS (times in usecs)
#define OUTPUT_ON_TIME 20
#define OUTPUT_OFF_TIME 70
#define ISO_PULSE false
#define CAP_VOLTAGE_SETPOINT 70         //Don't set higher than 65

//CC Charger Parameters (don't change unless you know what you're doing)
#define CHARGER_CURRENT 850

//Math stuff DO NOT EDIT
#define CAP_VOLTAGE_PWM_LEVEL CAP_VOLTAGE_SETPOINT * 303 / 48

bool os = false;

bool cut_on_off_irq(repeating_timer_t *rt) {

    if(!gpio_get(CUT_nEN_PIN) && !cutting_enabled) {
        cutting_enabled = true;
        begin_output_pulses(OUTPUT_ON_TIME, OUTPUT_OFF_TIME, ISO_PULSE);
    } else if(gpio_get(CUT_nEN_PIN)) {
        cutting_enabled == false;
    }

    if(gpio_get(CUT_nEN_PIN)) {
        cutting_enabled = false;
        disable_gate_driver();
    }

    gpio_put(1, os);
    os = !os;

    return true;

}

void default_gpio_callback(uint gpio, uint32_t event_mask) {
    gpio_acknowledge_irq(gpio, event_mask);
    return;
}

int main() {

    // stdio_init_all();                                                                    //Start the Serial console

    // gpio_init(CUT_nEN_PIN);
    // gpio_set_dir(CUT_nEN_PIN, GPIO_IN);
    // gpio_set_pulls(cut_NEN, true, false);

    sleep_ms(1000);

    // while(gpio_get(23)) {}

    CC_Charger_init(CHARGER_CURRENT, CAP_VOLTAGE_PWM_LEVEL);                                //Setup the CC Charger Outputs

    pulse_generator_init(10);

    gpio_init(CUT_nEN_PIN);
    gpio_set_dir(CUT_nEN_PIN, GPIO_IN);
    gpio_set_pulls(CUT_nEN_PIN, false, false);

    gpio_init(1);
    gpio_set_dir(1, GPIO_OUT);

    irq_set_enabled(IO_IRQ_BANK0, true);

    sleep_ms(1000);

    repeating_timer_t timer;

    add_repeating_timer_ms(100, cut_on_off_irq, NULL, &timer);

    while (true) {
        // sleep_us(10);
        // gpio_put(3, true);
        // sleep_us(10);
        // gpio_put(3, false);
    }
}