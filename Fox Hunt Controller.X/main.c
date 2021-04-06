/*
 * File:   main.c
 * Author: Macon Williams
 *
 * Created on March 22, 2021, 3:29 PM
 */

#include "config.h"
#include <xc.h>
#include "cw.h"

#define _XTAL_FREQ              31000UL         // Frequency of the low speed internal oscillator (31 kHz)

#define COS_STATUS              PORTAbits.RA0   // Carrier Operated Switch from Radio
#define PTT_PIN                 PORTAbits.RA1   // Raise high to turn on PTT

#define CALLSIGN                "N0MCW"         // Amateur radio callsign here
#define DIT_LENGTH              60              // CW speed, dit length in ms (roughly 20 wpm)
#define TONE_FREQUENCY          550             // CW Tone frequency in Hz

int dit_length;
bool fox_hunt, transmit_change, transmit;

void main(void) {
    initialize_pins();
    
    // Main loop
    while(true) {
        // Pause and wait for the user to key-up. Swap to doze mode to save power.
        CPUDOZEbits.DOZE    = 0b011;            // Doze ratio of 1:16 CPU instruction cycles
        CPUDOZEbits.DOZEN   = 1;                // Throw the CPU into doze mode, reduces power consumption (operates at roughly 1937 Hz)
        while(!COS_STATUS);                     // Wait for initial key-up
        while(COS_STATUS);                      // Wait for user to un-key
        CPUDOZEbits.DOZEN   = 0;                // Bring CPU out of doze mode
        
        initialize_tone_generator(_XTAL_FREQ, TONE_FREQUENCY);      // Start PWM module to generate the CW
        initialize_cw(DIT_LENGTH);                                  // Start the DSM module for CW timings
        fox_hunt, transmit_change, transmit = true;                 // Start off transmitting
        PTT_PIN = 1;
        cw_message("Good luck", 9);             // Wish our players good luck of course!
        deinitialize_cw();                      // Disable the DSM module when we're not using it
        
        // The "fox hunt" has begun!
        while(fox_hunt) {
            
            if (transmit_change) {
                if (transmit) {
                    PTT_PIN = 1;                // Turn on the carrier signal, writes to PORT register which should forward to LAT register
                } else {
                    initialize_cw(DIT_LENGTH);
                    cw_message(CALLSIGN, sizeof(CALLSIGN));         // Identify after every transmission to follow the rules
                    __delay_ms(500);
                    PTT_PIN = 0;                // Turn off the carrier signal, writes to PORT register which should forward to LAT register
                    deinitialize_cw();
                }
                transmit_change = false;        // Make sure we don't enter this if statement again until interrupt
            }

            // Stop the hunt if the radio receives a signal
            if (COS_STATUS) {
                while(COS_STATUS);              // Wait for user to unkey
                __delay_ms(500);
                initialize_cw(DIT_LENGTH);
                cw_message("OK", 2);            // User feedback, acknowledge the shutdown
                cw_message(CALLSIGN, sizeof(CALLSIGN));
                deinitialize_cw();
                deinitialize_tone_generator();
                fox_hunt = false;               // Leave the fox hunt loop and go back to dozing
            }
        }
    }
}

void initialize_pins(void) {
    ANSELA              = 0x00;     // Disable analog inputs on PORTA
    TRISAbits.TRISA0    = 1;        // Set RA0 to digital input, COS in
    TRISAbits.TRISA1    = 0;        // Set RA1 to digital output, PTT out
    TRISCbits.TRISC4    = 0;        // Set RC4 to digital output, CW out
}

void initialize_interrupts(void) {
    INTCON0bits.IPEN    = 0;        // All interrupts are high priority
    
}

void configure_timer(void) {
    T1CONbits.CKPS      = 0b01;     // 1:2 prescale
    T1CLKbits.CS        = 0b0001;   // Fosc/4
    T1GCONbits.GE       = 0;        // Gate disable, always counting
    
    
}

void __interrupt(high_priority) HighIsr(void) {
    if (TMR1IF) {
        transmit_change = true;
        transmit = !transmit;
        
        TMR1IF = 0;
    }
}