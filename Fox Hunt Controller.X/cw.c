/*
 * File: cw.c
 * 
 * Author: Macon Williams
 * Created: March 24th, 2021 12:10 AM
 */

#include "cw.h"

void initialize_cw() {
    MD1CON1bits.CHSYNC  = 0;
    MD1CON1bits.CLSYNC  = 0;
    MD1CARHbits.CH      = 0b01000;      // Carrier high is PWM5
    MD1SRCbits.MD1MS    = 0b00001;      // Modulating source is DSM1 bit
    RC4PPS              = 0b101000;     // DSM should output on port RC4
    MD1CON0bits.EN      = 1;            // Enable DSM
    T4CLKbits.T4CS0     = 1;            // Timer 4 oscillator set to Fosc/4
    T4CONbits.CKPS      = 0b110;        // Timer 4 1:64 prescaler set
    T4PRbits.PR4        = 0x7;          // Count up to 7 decimal (roughly 60 ms)
    T4HLTbits.MODE      = 0b01000;      // One shot timer
    
    tone_enable(true);
}

void deinitialize_cw() {
    MD1CON0bits.EN      = 0;
    tone_enable(false);
}

void cw_message(char message[], size_t message_size) {
    for (int i = 0; i < message_size; i++) {
        cw(message[i]);
    }
    dahspace(); dahspace();          // Space between each word
}

void cw(char character) {
  switch(character){
    
    // Letters
    case 'A': case 'a':
        dit(); ditspace(); dah(); dahspace();                                             break;      
    case 'B': case 'b':
        dah(); ditspace(); dit(); ditspace(); dit(); ditspace(); dit(); dahspace();       break;      
    case 'C': case 'c':
        dah(); ditspace(); dit(); ditspace(); dah(); ditspace(); dit(); dahspace();       break;      
    case 'D': case 'd':
        dah(); ditspace(); dit(); ditspace(); dit(); dahspace();                          break;      
    case 'E': case 'e':
        dit(); dahspace();                                                                break;      
    case 'F': case 'f':
        dit(); ditspace(); dit(); ditspace(); dah(); ditspace(); dit(); dahspace();       break;      
    case 'G': case 'g':
        dah(); ditspace(); dah(); ditspace(); dit(); dahspace();                          break;      
    case 'H': case 'h':
        dit(); ditspace(); dit(); ditspace(); dit(); ditspace(); dit(); dahspace();       break;      
    case 'I': case 'i':
        dit(); ditspace(); dit(); dahspace();                                             break;      
    case 'J': case 'j':
        dit(); ditspace(); dah(); ditspace(); dah(); ditspace(); dah(); dahspace();       break;      
    case 'K': case 'k':
        dah(); ditspace(); dit(); ditspace(); dah(); dahspace();                          break;      
    case 'L': case 'l':
        dit(); ditspace(); dah(); ditspace(); dit(); ditspace(); dit(); dahspace();       break;      
    case 'M': case 'm':
        dah(); ditspace(); dah(); dahspace();                                             break;      
    case 'N': case 'n':
        dah(); ditspace(); dit(); dahspace();                                             break;      
    case 'O': case 'o':
        dah(); ditspace(); dah(); ditspace(); dah(); dahspace();                          break;      
    case 'P': case 'p':
        dit(); ditspace(); dah(); ditspace(); dah(); ditspace(); dit(); dahspace();       break;      
    case 'Q': case 'q':
        dah(); ditspace(); dah(); ditspace(); dah(); dahspace();                          break;      
    case 'R': case 'r':
        dit(); ditspace(); dah(); ditspace(); dit(); dahspace();                          break;      
    case 'S': case 's':
        dit(); ditspace(); dit(); ditspace(); dit(); dahspace();                          break;      
    case 'T': case 't':
        dah(); dahspace();                                                                break;      
    case 'U': case 'u':
        dit(); ditspace(); dit(); ditspace(); dah(); dahspace();                          break;      
    case 'V': case 'v':
        dit(); ditspace(); dit(); ditspace(); dit(); ditspace(); dah(); dahspace();       break;
    case 'W': case 'w':
        dit(); ditspace(); dah(); ditspace(); dah(); dahspace();                          break;
    case 'X': case 'x':
        dah(); ditspace(); dit(); ditspace(); dit(); ditspace(); dah(); dahspace();       break;
    case 'Y': case 'y':
        dah(); ditspace(); dit(); ditspace(); dah(); ditspace(); dah(); dahspace();       break;
    case 'Z': case 'z':
        dah(); ditspace(); dah(); ditspace(); dit(); ditspace(); dit(); dahspace();       break;

    // Numbers
    case '1':
        dit(); ditspace(); dah(); ditspace(); dah(); ditspace(); dah(); ditspace(); dah(); dahspace();      break;
    case '2':
        dit(); ditspace(); dit(); ditspace(); dah(); ditspace(); dah(); ditspace(); dah(); dahspace();      break;
    case '3':
        dit(); ditspace(); dit(); ditspace(); dit(); ditspace(); dah(); ditspace(); dah(); dahspace();      break;
    case '4':
        dit(); ditspace(); dit(); ditspace(); dit(); ditspace(); dit(); ditspace(); dah(); dahspace();      break;
    case '5':
        dit(); ditspace(); dit(); ditspace(); dit(); ditspace(); dit(); ditspace(); dit(); dahspace();      break;
    case '6':
        dah(); ditspace(); dit(); ditspace(); dit(); ditspace(); dit(); ditspace(); dit(); dahspace();      break;
    case '7':
        dah(); ditspace(); dah(); ditspace(); dit(); ditspace(); dit(); ditspace(); dit(); dahspace();      break;
    case '8':
        dah(); ditspace(); dah(); ditspace(); dah(); ditspace(); dit(); ditspace(); dit(); dahspace();      break;
    case '9':
        dah(); ditspace(); dah(); ditspace(); dah(); ditspace(); dah(); ditspace(); dit(); dahspace();      break;
    case '0':
        dah(); ditspace(); dah(); ditspace(); dah(); ditspace(); dah(); ditspace(); dah(); dahspace();      break;

    // Characters
    case '.':
        dit(); ditspace(); dah(); ditspace(); dit(); ditspace(); dah(); ditspace(); dit(); ditspace(); dah(); dahspace();       break;
    case ',':
        dah(); ditspace(); dah(); ditspace(); dit(); ditspace(); dit(); ditspace(); dah(); ditspace(); dah(); dahspace();       break;
    case '?':
        dit(); ditspace(); dit(); ditspace(); dah(); ditspace(); dah(); ditspace(); dit(); ditspace(); dit(); dahspace();       break;
    case ':':
        dah(); ditspace(); dah(); ditspace(); dah(); ditspace(); dit(); ditspace(); dit(); ditspace(); dit(); dahspace();       break;
    case '/':
        dah(); ditspace(); dit(); ditspace(); dit(); ditspace(); dah(); ditspace(); dit(); dahspace();                          break;
    case '-':
        dah(); ditspace(); dit(); ditspace(); dit(); ditspace(); dit(); ditspace(); dit(); ditspace(); dah(); dahspace();       break;
    case '=':
        dah(); ditspace(); dit(); ditspace(); dit(); ditspace(); dit(); ditspace(); dah(); dahspace();                          break;
    case '\'':
        dit(); ditspace(); dah(); ditspace(); dah(); ditspace(); dah(); ditspace(); dah(); ditspace(); dit(); dahspace();       break;
    case '(': case ')':
        dah(); ditspace(); dit(); ditspace(); dah(); ditspace(); dah(); ditspace(); dit(); ditspace(); dah(); dahspace();       break;
    case '_':
        dit(); ditspace(); dit(); ditspace(); dah(); ditspace(); dah(); ditspace(); dit(); ditspace(); dah(); dahspace();       break;
    case '!':
        dah(); ditspace(); dit(); ditspace(); dah(); ditspace(); dit(); ditspace(); dah(); ditspace(); dah(); dahspace();       break;
    case '&':
        dit(); ditspace(); dah(); ditspace(); dit(); ditspace(); dit(); ditspace(); dit(); dahspace();                          break;
    case '\"':
        dit(); ditspace(); dah(); ditspace(); dit(); ditspace(); dit(); ditspace(); dah(); ditspace(); dit(); dahspace();       break;
    case ';':
        dah(); ditspace(); dit(); ditspace(); dah(); ditspace(); dit(); ditspace(); dah(); ditspace(); dit(); dahspace();       break;
    case '$':
        dit(); ditspace(); dit(); ditspace(); dit(); ditspace(); dah(); ditspace(); dit(); ditspace(); dit(); ditspace(); dah(); dahspace();      break;
    case ' ':
        dahspace(); dahspace();
        break;

    // No valid character:
    default:
        dahspace(); dahspace();
  }
}

void dah() {
    MD1CON0bits.BIT     = 1;
    dahspace();
    MD1CON0bits.BIT     = 0;
}

void dit() {
    MD1CON0bits.BIT     = 1;
    ditspace();
    MD1CON0bits.BIT     = 0;
}

void ditspace() {
    T4CONbits.ON        = 1;        // Start timer 4
    while(T4CONbits.ON);            // Wait for the postscaler select bit
}

void dahspace() {
    for (int i = 0; i < 3; i++) {
        ditspace();
    }
}