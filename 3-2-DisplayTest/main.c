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

//Array Length Helper
#define ARRAY_LEN(arr) (sizeof(arr)/sizeof(arr[0]))

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
    //Variables
    unsigned int colourIdx = 0;
    unsigned short colours[6] =
        {LT24_RED,LT24_YELLOW,LT24_GREEN,LT24_CYAN,LT24_BLUE,LT24_MAGENTA};
    //Initialise the LCD Display.
    exitOnFail(
            LT24_initialise(0xFF200060,0xFF200080), //Initialise LCD
            LT24_SUCCESS);                          //Exit if not successful
    HPS_ResetWatchdog();
    //Display Internal Test Pattern
    exitOnFail(
           LT24_testPattern(), //Generate test pattern
           LT24_SUCCESS);      //Exit if not successful
    HPS_ResetWatchdog();
    //Wait a moment
    usleep(500000);
    HPS_ResetWatchdog();
    //Main Run Loop
    while (1) {
    	LT24_clearDisplay(LT24_BLACK);
    	LT24_drawPixel(LT24_RED,10,10);
        //Set display to solid colour
        //LT24_clearDisplay(colours[colourIdx]);
        //Update Colour Index
        //colourIdx++;
        //if (colourIdx >= ARRAY_LEN(colours)) {
        //    colourIdx = 0;
        //}
        //Finally reset the watchdog.
        HPS_ResetWatchdog();
    }
}
