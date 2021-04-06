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

bool fox_hunt, transmit_change, transmit;
void initialize_interrupts(void);
void initialize_pins(void);
void configure_timer(void);

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
        fox_hunt = true;
        transmit_change = true; 
        transmit = true;                        // Start off transmitting
        PTT_PIN = 1;
        cw_message("Good luck", 9);             // Wish our players good luck of course!
        deinitialize_cw();                      // Disable the DSM module when we're not using it
        initialize_interrupts();                // Configure interrupts
        configure_timer();                      // Initialize timer 1 (60 seconds transmit, 60 seconds off)
        
        // The "fox hunt" has begun!
        while(fox_hunt) {
            
            if (transmit_change) {
                T1CONbits.ON    = 0;            // Turn timer 1 off
                if (transmit) {
                    PTT_PIN = 1;                // Turn on the carrier signal, writes to PORT register which should forward to LAT register
                } else {
                    initialize_cw(DIT_LENGTH);
                    cw_message(CALLSIGN, sizeof(CALLSIGN));         // Identify after every transmission
                    __delay_ms(500);
                    PTT_PIN = 0;                // Turn off the carrier signal, writes to PORT register which should forward to LAT register
                    deinitialize_cw();
                }
                transmit_change = false;    // Make sure we don't enter this if statement again until interrupt
                TMR1H           = 0x1C;     // Timer 1 high byte (set to run for 60 seconds before overflowing)
                TMR1L           = 0xF2;     // Timer 1 low byte (set to run for 60 seconds before overflowing)
                T1CONbits.ON    = 1;        // Turn timer 1 on
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
    PTT_PIN             = 0;
    
}

void initialize_interrupts(void) {
    INTCON0bits.IPEN    = 0;        // All interrupts are high priority
    INTCON0bits.GIE     = 1;        // Enable interrupts
    PIE4bits.TMR1IE     = 1;        // Enable Timer 1 interrupt
    TMR1IF              = 0;        // Clear Timer 1 interrupt
}

void configure_timer(void) {
    T1CONbits.CKPS      = 0b11;     // 1:8 prescale
    T1CLKbits.CS        = 0b0001;   // Fosc/4
    T1GCONbits.GE       = 0;        // Gate disable, always counting
}

void __interrupt(irq(IRQ_TMR1)) TMR1_ISR(void) {
    TMR1IF              = 0;            // Clear the interrupt
    transmit_change     = true;
    transmit            = !transmit;    // Toggle the transmitter
}
