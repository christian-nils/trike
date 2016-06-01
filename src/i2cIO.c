/*****************************************************************************
* © 2014 Microchip Technology Inc. and its subsidiaries.
* You may use this software and any derivatives exclusively with
* Microchip products.
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".
* NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
* INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
* AND FITNESS FOR A PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP
* PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.
* TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL
* CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF
* FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
* MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE
* OF THESE TERMS.
*****************************************************************************/
/** @file  i2cIO.c
*   I2C functions to I/F PIC24 with SSC7150
*****************************************************************************
*  MM7150 with Explorer 16 Development Board Sample Code i2c i/o file
*
*   Company : Microchip Technology Inc.
*
*   File name : i2cIO.c
*
*   Summary : Module for MM7150 demo code which interfaces 
*           PIC24 (Explorer16 board) to SSC7150 via i2c bus/calls 
*   
*   Functions : i2c_cmd_WrRd
*               gets_I2C1
*               i2cIO_error
*
*   Revisions : 0.2 9-18-14 C21674 - enhanced error handling, added timer2 interrupt 
*                                    to exit for unresponsive i2c, gets_I2C1 check status
*                                    bits on exit with no data 
*             : 0.1 8-4-14 C21674
*             : 0.0 7-1-14 C16368  - Initial version created
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

    UINT16 i = 0;                                               
    UINT8 ucSize = 1;                                                // Set return value for size of data read for bAdjust=FALSE
    UINT16 usStat = 0;
	UINT32 ret;
		
	if (!bAdjust){
		ret = i2c_smbus_read_i2c_block_data(SLAVE_FD,
			0, usLength, ucRdptr); // you specify explicitly the length
	} else{
		ret = i2c_smbus_read_block_data(SLAVE_FD,
			0, ucRdptr); //automatically get the number of bytes to read (up to 32bytes)
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
	printf("i2c_cmd_WrRd\n");
    if (ucBytes_wr > BUF_150)                                       // sanity check for maximum buffer size
        return I2C_BUF_OVRFLO;                                      // return i2c buffer overflow error code to calling routine

    switch(ucCmd)
    {
        case WRITE:
//s32 i2c_smbus_write_i2c_block_data(struct i2c_client *client, // you can control the lenght of the tx data. Without sending the count to the slave device.
//					   u8 command, u8 length,
//					   const u8 *values);
			if(i2c_smbus_write_i2c_block_data(SLAVE_FD, // you can control the lenght of the tx data. Without sending the count to the slave device.
												0, ucBytes_wr, &ucData_wr)){
                        i2cIO_error(WRITE_COLL);      //FIXME         
					   }
                    
            break;

        case READ:

			gets_I2C(ucData_rd, usBytes_rd, bAdjust);          // Read in multiple bytes

            break;

        case WR_RD:
			if(i2c_smbus_write_i2c_block_data(SLAVE_FD, // you can control the lenght of the tx data. Without sending the count to the slave device.
							0, ucBytes_wr, &ucData_wr))
			{
				i2cIO_error(WRITE_COLL);      //FIXME         
			}
            gets_I2C(ucData_rd, usBytes_rd, bAdjust);              // Read in multiple bytes
            
            break;
    }

//    StopI2C1();                                                     // Send STOP condition
    
//    while (I2C1CONbits.PEN );                                       // Wait until stop condition is over
//    
//    MasterWaitForIntrI2C1();                                        // Wait for interrupt request
//   
//    StopI2CTimer();                                                 //turn off timer2 interrupt 

    return I2C_SUCCESS;
}
