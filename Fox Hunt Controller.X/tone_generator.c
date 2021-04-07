/*
 * File: tone_generator.c
 * Author: Macon Williams
 * 
 * Date: March 24th, 2021 11:03PM
 */


#include "tone_generator.h"

void initialize_tone_generator(float osc_freq, float pwm_freq) {
    unsigned char period_value = (((1/pwm_freq) * osc_freq)/4) - 0.5;      // Assumes a prescale of 1:1
    unsigned char duty_cycle = (2 * (period_value + 1)) + 0.5;             // 50% duty cycle
    unsigned char duty_cycle_l;
    
    unsigned mask;
    mask = (1 << 2) - 1;
    duty_cycle_l = duty_cycle & mask;
    
    TRISCbits.TRISC2    = 1;                // RC2 set to input (disabled), RC2 holds CCP1
    PWM5CON             = 0x00;             // Clear the PWM5CON register
    T2PR                = period_value;     // T2PR PWM period value, calculated above
    PWM5DCLbits.DC      = duty_cycle_l;
    PWM5DCHbits.DC      = duty_cycle >> 2;
    
    PIR4bits.TMR2IF = 0;        // Clear TMR2 interrupt flag
    T2CLKbits.CS0   = 1;        // Set timer frequency to Fosc/4
    T2CONbits.CKPS  = 0b000;    // Pre scaler 1:1
    T2CONbits.ON    = 1;        // Timer 2 on
    
    while(!PIR4bits.TMR2IF);    // Wait for timer 2 to overflow
}

void deinitialize_tone_generator() {
    PWM5CON             = 0;        // PWM5 control register cleared
    T2CONbits.ON        = 0;        // Disable Timer 2
}

void tone_enable(bool tone_active) {
    if (tone_active) {
        PWM5CONbits.EN = 1;         // Enable PWM5
    } else {
        PWM5CONbits.EN = 0;         // Disable PWM5
    }
}