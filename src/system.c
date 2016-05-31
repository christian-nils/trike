#include "../headers/app.h"

volatile BOOL EC_DATA_AVAIL = FALSE;  
volatile int SLAVE_FD;
struct timeval POR_TIMER;													// amount of elapsed time (in ms) since POR 

void sys_init(void){	
	
	gettimeofday(&POR_TIMER, 0);					//reset POR timer
	
	wiringPiSetup(); //initialize wiringPi, using wiringPi pin numbering (see: http://wiringpi.com/reference/setup/, https://projects.drogon.net/raspberry-pi/wiringpi/pins/) 
	interrupts_init(); // set up all the interrupts
	SLAVE_FD = wiringPiI2CSetup(SLAVE_ADDR); // configure the i2c communication
	printf("IMU set up , FID: %i\n", SLAVE_FD);
	
	Wake_init();	//Initialize the wake and reset signal	
	Reset_init();
	return SUCCESS;
}

/** Reset_init
* @note	config wake on reset pin as output, set LOW initially
* @param 
* @return 
*/ 
void Reset_init()
{
    // configure pin where the wake signal is connected
	pinMode(3, OUTPUT); //Set the GPIO pin 2 to OUTPUT
	pullUpDnControl(3, PUD_UP); //Set the GPIO pin 2 to a pull-up resistor
	digitalWrite(3, 0) ; //set the signal to LOW
}

/** Wake_init
* @note	config wake on wake pin as output, set HIGH initially
* @param 
* @return 
*/ 
void Wake_init()
{
    // configure pin where the wake signal is connected
	pinMode(2, OUTPUT); //Set the GPIO pin 2 to OUTPUT
	pullUpDnControl(2, PUD_UP); //Set the GPIO pin 2 to a pull-up resistor
	digitalWrite(2, 1) ; //set the signal to HIGH
}

/** Wake_signal
* @note	assert wake signal on wake pin, wait 1 ms, deassert
* @param 
* @return 
*/ 
void Wake_signal()
{

	digitalWrite(2, 0) ; //set the signal to LOW                                            //assert wake signal
    delay(2);                                                       //spec says 3µs assertion, let's use ms delay and wait ~2 ms
    digitalWrite(2, 1) ; //set the signal to HIGH                                             //de-assert wake signal

}

int interrupts_init(void){
	int ret;
	
	pinMode(0, INPUT); //Alert line gpio is an input
	// Initialize the EC_DATA_AVAIL value
	if (digitalRead(0)==0){
		EC_DATA_AVAIL = FALSE;
	}
	else {
		EC_DATA_AVAIL = TRUE;
	}
	// Place here the interrupt function to switch EC_DATA_AVAIL to TRUE when data are present
	if (ret = wiringPiISR (0, INT_EDGE_BOTH,  &data_available_interrupt)) //EC_DATA_AVAIL = TRUE/FALSE; true if edge falling
		return ret; //if not success, return error
		
	return SUCCESS;
}

void data_available_interrupt(void) {
	
	if (!EC_DATA_AVAIL)                                         // If a falling edge occurred (data is available from EC)
        {
            EC_DATA_AVAIL = TRUE;                                   // Toggle EC_DATA_AVAIL flag to notify data received
        }
        else
        {
            EC_DATA_AVAIL = FALSE;                                  // interrupt de-asserted
        }
}