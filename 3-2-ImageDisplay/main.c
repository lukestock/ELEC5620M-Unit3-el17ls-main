/*
 * DE1-SoC LT24 Example
 *
 * Displays a test pattern on the LT24
 * LCD using the LT24 bare metal driver.
 *
 *  Created on: 09 Feb 2018
 */

#include "DE1SoC_LT24/DE1SoC_LT24.h"
#include "HPS_Watchdog/HPS_Watchdog.h"
#include "HPS_usleep/HPS_usleep.h"

//Include our image
#include "Test.h"
#include "Alien.h"
#include "rbdots.h"
#include "rbpanels.h"

//Debugging Function (same as last lab)
#include <stdlib.h>
void exitOnFail(signed int status, signed int successStatus){
    if (status != successStatus) {
        exit((int)status); //Add breakpoint here to catch failure
    }
}

//
// Main Function
// =============
int main(void) {
	// Define bass addresses
	volatile unsigned int *key_ptr = (unsigned int *)0xFF20005C;  // base address for rising edge of key press
    //Initialise the LCD Display.
    exitOnFail(
            LT24_initialise(0xFF200060,0xFF200080), //Initialise LCD
            LT24_SUCCESS);                          //Exit if not successful
    HPS_ResetWatchdog();
    //Display the image. Setting the co-ordinates and size of the image.
   // exitOnFail(
    //        LT24_copyFrameBuffer(alien,0,0,40,40),
    //        LT24_SUCCESS); //Exit if not successful



    //Main Run Loop
    while (1) {
        // Define images
        const unsigned short rbdots[76800];
    	const unsigned short alien[40][40];
    	unsigned int x, y;
    	LT24_copyFrameBuffer(rbdots,0,0,240,320);

    	for (x = 0; x < 40; x++) {
    		for(y = 0; y < 40; y++){
    			// gets a section of 5 x 10 image starting from flappy_background[0][0]
    			rbdots[y][x] = alien[0 + y][0 + x];

    		}
    	}

    	LT24_copyFrameBuffer(&alien,0,0,40,40);


    	/*
    	if (*key_ptr & 0x1) {
    	    	LT24_copyFrameBuffer(Test,0,0,240,320);
    	  }
		  LT24_clearDisplay(0x000);
    	  if (*key_ptr & 0x2) {
    	    	LT24_copyFrameBuffer(rbdots,0,0,240,320);
    	  }
		  LT24_clearDisplay(0x000);
    	  if (*key_ptr & 0x4) {
    	    	LT24_copyFrameBuffer(rbscreen,0,0,240,320);
    	  }
		  LT24_clearDisplay(0x000);
    	  if (*key_ptr & 0x8) {
    	   LT24_copyFrameBuffer(alien,0,0,40,40);
    	  }

		  LT24_clearDisplay(0x000);
*/
        HPS_ResetWatchdog(); //Just reset the watchdog.
    }
}
