#include "../headers/app.h"

volatile BOOL EC_DATA_AVAIL = FALSE;  

int sys_init(void){	
	int SLAVE_FD = wiringPiI2CSetup(SLAVE_ADDR);
	printf("%i\n", SLAVE_FD);
	if (SLAVE_FD<0) // Error
		return errno;
		
	return SUCCESS;
}

int interrupt_init(void){
	// Place here the interrupt function to switch EC_DATA_AVAIL to TRUE when data are present
}