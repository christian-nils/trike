/** @file  main_cn.c
*****************************************************************************
* 	First main
*
*   Company : Chalmers University
*
*   File name : main_cn.c
*
*   Summary : 
*
*   Functions : main
*               
*   Revisions : 
* 
******************************************************************************/

//****************************************************************************
//****************************************************************************
//  Section : Included files
//****************************************************************************
//****************************************************************************

#include "../headers/main_cn.h"


//*****************************************************************************
//*****************************************************************************
//  Section : File scope variables and functions
//*****************************************************************************
//*****************************************************************************

//*****************************************************************************
//*****************************************************************************
//  Section : Main Entry Point
//*****************************************************************************
//*****************************************************************************

/** main
* @note	Initialize sensor 
* @param 
* @return error status
*/ 

int main(void) 
{
	int SLAVE_FD = wiringPiI2CSetup(SLAVE_ADDR);
//	printf("%d", SLAVE_FD);
	cout << "Init result: "<< SLAVE_FD << endl;
	cout << "Trying to read from IMU" << endl;
}