#include "../headers/app.h"

volatile BOOL EC_DATA_AVAIL = FALSE;  
volatile int SLAVE_FD;
clock_t POR_TIMER;													// amount of elapsed time (in ms) since POR 

void sys_init(void){	
	
	POR_TIMER = clock();					//reset POR timer
	
	wiringPiSetupGpio(); //initialize wiringPi, using GPIO pin numbering (see: http://wiringpi.com/reference/setup/, https://projects.drogon.net/raspberry-pi/wiringpi/pins/) 
	
	init_I2C();
//	i2c_get_address();
	Reset_init();
	Wake_init();	//Initialize the wake and reset signal	
//	Wake_signal();
//	
	interrupts_init(); // set up all the interrupts
	delay(5000);
//	SLAVE_FD = wiringPiI2CSetup(SLAVE_ADDR); // configure the i2c communication
//	printf("IMU set up , FID: %i\n", SLAVE_FD);
	
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
	pinMode(RESPIN, OUTPUT); //Set the GPIO pin 2 to OUTPUT
	digitalWrite(RESPIN, 0); //set the signal to LOW
	delay(2); //hold the reset for 2ms
	digitalWrite(RESPIN, 1) ; //set the signal to HIGH 
}

/** Wake_init
* @note	config wake on wake pin as output, set HIGH initially
* @param 
* @return 
*/ 
void Wake_init()
{
    // configure pin where the wake signal is connected
	pinMode(WAKPIN, OUTPUT); //Set the GPIO pin 2 to OUTPUT
	digitalWrite(WAKPIN, 1) ; //set the signal to HIGH
}

/** Wake_signal
* @note	assert wake signal on wake pin, wait 1 ms, deassert
* @param 
* @return 
*/ 
void Wake_signal()
{

	digitalWrite(WAKPIN, 0) ; //set the signal to LOW                                            //assert wake signal
    delay(2);                                                       //spec says 3µs assertion, let's use ms delay and wait ~2 ms
    digitalWrite(WAKPIN, 1) ; //set the signal to HIGH                                             //de-assert wake signal

}

int interrupts_init(void){
	int ret;
	
	pinMode(INTPIN, INPUT); //Alert line gpio is an input	
	pullUpDnControl(INTPIN, PUD_UP); //Set the GPIO pin 2 to a pull-up resistor
	// Initialize the EC_DATA_AVAIL value
	if (digitalRead(INTPIN)==0){
		EC_DATA_AVAIL = TRUE;
	}
	else {
		EC_DATA_AVAIL = FALSE;
	}
	// Place here the interrupt function to switch EC_DATA_AVAIL to TRUE when data are present
	ret = wiringPiISR(INTPIN, INT_EDGE_BOTH,  &data_available_interrupt);
	
//	if (ret = wiringPiISR (0, INT_EDGE_BOTH,  &data_available_interrupt)) //EC_DATA_AVAIL = TRUE/FALSE; true if edge falling
//		{printf("%f", ret);
//		return ret; //if not success, return error
//		}
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

void MasterWaitForIntrI2C(void){
	printf("%d\n", EC_DATA_AVAIL);
	while(!EC_DATA_AVAIL);
	EC_DATA_AVAIL = FALSE;	
}