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

#include "../headers/app.h"


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
	SLAVE_FD = wiringPiI2CSetup(SLAVE_ADDR);
	cout << "Init result: "<< SLAVE_FD << endl;

}