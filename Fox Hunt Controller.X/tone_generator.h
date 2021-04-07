/* 
 * File:   tone_generator.h
 * Author: Macon Williams
 *
 * Created on March 24, 2021, 11:01 PM
 */

#ifndef TONE_GENERATOR_H
#define	TONE_GENERATOR_H

#include <xc.h>
#include <stdbool.h>

void initialize_tone_generator(float osc_freq, float pwm_freq);

void deinitialize_tone_generator(void);

void tone_enable(bool tone_active);

#endif	/* TONE_GENERATOR_H */