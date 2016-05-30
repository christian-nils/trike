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
	UINT8 ucTx_data[BUF_40];  
	UINT8 ucCmdDatbuf[BUF_40];  
	HID_DESCRIPTOR_TABLE HID_FIELD;                                     //structure for HID Descriptor 
	int SLAVE_FD = wiringPiI2CSetup(SLAVE_ADDR);
//	printf("%d", SLAVE_FD);
	cout << "Init result: "<< SLAVE_FD << endl;
	cout << "Trying to power the IMU" << endl;
	ucTx_data[0] = HID_FIELD.wCmdReg;                       //command field bytes from HID config table 
	ucTx_data[1] = (HID_FIELD.wCmdReg >> BYTE_SHIFT);                              
	ucTx_data[2] = POWER_CMD_LSB;                           //HID Power command opcode low  byte for POWER Device ON
	ucTx_data[3] = POWER_CMD_MSB;                           //HID Power command opcode high byte

	ucRetStat = i2c_cmd_WrRd (WRITE,                        // Issue power on command to SSC7150
						CMD_LEN,                            //num of cmd bytes
						ucTx_data,                          //cmd buf
						0,                                  //num of bytes to read
						ucCmdDatbuf,                        //recv buf
						FALSE);                             //flag indicating that we specified the number of bytes to read explicitly

	if (ucRetStat != 0)
		cout << "Fail" << endl;
		return ucRetStat;                               // command failed
//	cout << wiringPiI2CRead(SLAVE_FD) << endl;
//	perror("wiringPiI2CRead()");
}