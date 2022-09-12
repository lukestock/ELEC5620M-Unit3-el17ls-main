/*
 * DE1-SoC Audio Example
 *
 * Generates a sinusoidal tone waveform
 * Writes stereo audio to the CODEC (LINE OUT)
 *
 *  Created on: 03 Feb 2018
 */

#include "DE1SoC_WM8731/DE1SoC_WM8731.h"
#include "HPS_Watchdog/HPS_Watchdog.h"

//Include Floating Point Math Libraries
#include <math.h>

//Debugging Function (same as last lab)
#include <stdlib.h>
void exitOnFail(signed int status, signed int successStatus){
    if (status != successStatus) {
        exit((int)status); //Add breakpoint here to catch failure
    }
}

//Define some useful constants
#define F_SAMPLE 48000.0        //Sampling rate of WM8731 Codec (Do not change)
#define PI2      6.28318530718  //2 x Pi      (Apple or Peach?)

//
// Main Function
// =============
int main(void) {
    //Pointers
    volatile unsigned int*  LEDR_ptr  = (unsigned int *) 0xFF200000; // Red LEDs base address
    volatile unsigned char* fifospace_ptr;
    volatile unsigned int*  audio_left_ptr;
    volatile unsigned int*  audio_right_ptr;
    //Variables
    //Phase Accumulator
    double phase1 = 0.0;  // Phase accumulator
    double inc1   = 0.0;  // Phase increment
    double ampl1  = 0.0;  // Tone amplitude (i.e. volume)
    double phase2 = 0.0;  // Phase accumulator
    double inc2   = 0.0;  // Phase increment
    double ampl2  = 0.0;  // Tone amplitude (i.e. volume)
    signed int audio_sample1 = 0;
    signed int audio_sample2 = 0;
    //Initialise the Audio Codec.
    exitOnFail(
            WM8731_initialise(0xFF203040),  //Initialise Audio Codec
            WM8731_SUCCESS);                //Exit if not successful
    //Clear both FIFOs
    WM8731_clearFIFO(true,true);
    //Grab the FIFO Space and Audio Channel Pointers
    fifospace_ptr = WM8731_getFIFOSpacePtr();
    audio_left_ptr = WM8731_getLeftFIFOPtr();
    audio_right_ptr = WM8731_getRightFIFOPtr();
    //Initialise Phase Accumulator

    // For different frequencies refer to site: https://pages.mtu.edu/~suits/notefreqs.html
    inc1   = 110.0 * PI2 / F_SAMPLE;  // Calculate the phase increment based on desired frequency - e.g. 440Hz
    ampl1  = 8388608.0;               // Pick desired amplitude (e.g. 2^23). WARNING: If too high = deafening!
    phase1 = 0.0;
    inc2   = 440.0 * PI2 / F_SAMPLE;  // Calculate the phase increment based on desired frequency - e.g. 440Hz
    ampl2  = 8388608.0;               // Pick desired amplitude (e.g. 2^23). WARNING: If too high = deafening!
    phase2 = 0.0;
    // Primary function while loop
    while (1) {
        //Always check the FIFO Space before writing or reading left/right channel pointers
        if ((fifospace_ptr[2] > 0) && (fifospace_ptr[3] > 0)) {
            //If there is space in the write FIFO for both channels:
            //Increment the phase
            phase1 = phase1 + inc1;
            phase2 = phase2 + inc2;
            //Ensure phase is wrapped to range 0 to 2Pi (range of sin function)
            while (phase1 >= PI2 ) {
                phase1 = phase1 - PI2;
            }
            while (phase2 >= PI2 ) {
                phase2 = phase2 - PI2;
            }
            // Calculate next sample of the output tone.
            audio_sample1 = (signed int)( ampl1 * sin( phase1 ) );
            audio_sample2 = (signed int)( ampl2 * sin( phase2 ) );
            // Output tone to left and right channels.
            *audio_left_ptr = audio_sample1;
            *audio_right_ptr = audio_sample2;
        }
        //Debugging - display FIFO space on red LEDs.
        *LEDR_ptr = fifospace_ptr[2];  // Output 'WSRC' register to the red LEDs.
        //Finally reset the watchdog.
        HPS_ResetWatchdog();
    }
}
