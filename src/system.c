#include "../headers/app.h"

int sys_init(void){	
	int SLAVE_FD = wiringPiI2CSetup(SLAVE_ADDR);
	if (SLAVE_FD<0) // Error
		return errno;
		
	return SUCCESS;
}