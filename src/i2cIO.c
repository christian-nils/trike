/** @file  i2cIO.c
*   I2C functions to RPI with MM7150
*****************************************************************************
*  MM7150 with Raspberry Pi i2c i/o file
*
*   Company : Chalmers University
* 
* 	Author : Christian-Nils Boda (boda@chalmers.se)
*
*   File name : i2cIO.c
*
*   Summary : I2C interface between MM7150 and Raspberry Pi 
*   
*   Functions : i2c_cmd_WrRd
*               gets_I2C
*
*   Revisions : 0.0 2016-06-02: Initial file based on Microchip's functions
******************************************************************************/

//****************************************************************************
//****************************************************************************
//  Section : Included files
//****************************************************************************
//****************************************************************************

#include "../headers/app.h"

//*****************************************************************************
//*****************************************************************************
//  Section : Code
//*****************************************************************************
//*****************************************************************************

extern volatile int SLAVE_FD;

/** gets_I2C
* @note	Read bytes from sensor device via i2c channel slave
* @param ucRdptr pointer to data buffer from slave
* @param usLength number of bytes to read from slave
* @param bAdjust  first bytes of rx packet has number of bytes returned by read, if =TRUE use this new data length 
* @return 
*/ 
void gets_I2C(UINT8 *ucRdptr, UINT16 usLength, BOOL bAdjust){

	UINT32 ret;
	int i;
	
	if (!bAdjust){
		ret = i2c_smbus_read_i2c_block_data	(SLAVE_FD, (UINT8)0xFF, usLength, ucRdptr); // you specify explicitly the length
	} else{
		ret = i2c_smbus_read_block_data		(SLAVE_FD, (UINT8)0xFF, ucRdptr); //automatically get the number of bytes to read (up to 32bytes)
	}      
	for (i=0; i<usLength; i++){
		printf("%d\n", ucRdptr[i]);
	}
}
  

/** i2c_cmd_WrRd
* @note	i2c write,read, and combined write/read commands, start timer2 interrupt to exit on unresponsive i2c bus
* @param ucCmd error code
* @param ucBytes_wr Number of bytes to write to slave 
* @param ucData_wr Pointer to data buffer to send to slave 
* @param usBytes_rd Number of bytes to read from slave
* @param ucData_rd Pointer to data buffer from slave
* @param bAdjust Use 1st 2 bytes returned as new length (=TRUE) 
* @return I2C_SUCCESS(=0), I2C_BUF_OVRFLO(=0x22)
*/
UINT8 i2c_cmd_WrRd(UINT8 ucCmd, UINT8 ucBytes_wr,  UINT8 *ucData_wr, UINT16 usBytes_rd,  UINT8 *ucData_rd, BOOL bAdjust)
{        
	
    if (ucBytes_wr > BUF_150)                                       // sanity check for maximum buffer size
        return I2C_BUF_OVRFLO;                                      // return i2c buffer overflow error code to calling routine

    switch(ucCmd)
    {
        case WRITE:
		// you can control the length of the tx data. Without sending the count to the slave device.
			if(i2c_smbus_write_i2c_block_data(SLAVE_FD, 0, ucBytes_wr, ucData_wr))
				perror("Error while i2c writing:");  
					   
            break;

        case READ:

			gets_I2C(ucData_rd, usBytes_rd, bAdjust);          // Read in multiple bytes

            break;

        case WR_RD:
		
			if(i2c_smbus_write_i2c_block_data(SLAVE_FD, (UINT8)0xFF, ucBytes_wr, ucData_wr))			
				perror("Error while i2c writing:");           
			
            gets_I2C(ucData_rd, usBytes_rd, bAdjust);              // Read in multiple bytes
            
            break;
    }

    return I2C_SUCCESS;
}
