/* 
 * File:   cw.h
 * Author: Macon Williams
 *
 * Created on March 24, 2021, 9:51 AM
 * 
 * TODO: Add documentation
 */

#include "tone_generator.h"
#include <stddef.h>

#ifndef CW_H
#define	CW_H

int dit_length;

void initialize_cw(int speed);

void deinitialize_cw(void);

void cw_message(char callsign[], size_t message_size);

void cw(char character);

void dah(void);

void dit(void);

void ditspace(void);

void dahspace(void);


#endif	/* CW_H */

