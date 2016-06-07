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


    UINT16 i = 0;                                               
    UINT8 ucSize = 1;                                                // Set return value for size of data read for bAdjust=FALSE
    UINT16 usStat = 0;   


    while (usLength--)
    {		                                     
		if (usLength==1){
			ucRdptr[i++] = i2c_read_byte(NACK , FALSE) ;// Read in the byte received from slave
		} else {
			ucRdptr[i++] = i2c_read_byte(ACK , FALSE) ;
		}
        if (bAdjust && i == 2)                                      // Read first 2 bytes which have the length of the packet
        {
            usLength = ((ucRdptr[1] << BYTE_SHIFT) | ucRdptr[0]) - 2; // Actual length is value of first 2 bytes minus 2 (because we read 2 already)
            ucSize = usLength;
        }
    }

//    if (!ucSize)
//    {
//        usStat = I2C1STAT;                                          //read status register
//        StopI2CTimer();                                             //turn off timer2 interrupt
//
//        if (usStat & ACKSTAT_bit)
//            i2cIO_error(NOT_ACK);                                   //ACK error (does not return)
//
//        else if (usStat & BCL_bit)
//            i2cIO_error(BUS_COLL);                                  //BCL error (does not return)
//
//        else if (usStat & IWOL_bit)
//            i2cIO_error(WRITE_COLL);                                //WCL error (does not return)
//
//        else if (usStat & I2COV_bit)
//            i2cIO_error(RX_OVRFLO);                                 //Rx OVRFLO error (does not return)
//        
//        else
//            error_handler("i2c ",0, I2C_ERROR);                     //flag as general i2c error (does not return)
//    }
  
//	UINT8 cmd[0];
//	UINT32 ret;
//	int i;
//	
//	if (!bAdjust){
//		ret = i2c_smbus_read_i2c_block_data	(SLAVE_FD, cmd, usLength, ucRdptr); // you specify explicitly the length
//	} else{
//		ret = i2c_smbus_read_block_data		(SLAVE_FD, cmd, ucRdptr); //automatically get the number of bytes to read (up to 32bytes)
//	}      
//	for (i=0; i<usLength; i++){
//		printf("%d\n", ucRdptr[i]);
//	}
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
//	UINT32 ret;
	int i;
//	UINT8 cmd;
    if (ucBytes_wr > BUF_150)                                       // sanity check for maximum buffer size
        return I2C_BUF_OVRFLO;                                      // return i2c buffer overflow error code to calling routine

//	StartI2CTimer();                                                // start timer2 interrupt in case i2c hangs in 'while loop' functions
   
//    IdleI2C1();                                                     // Ensure module is idle
//    i2c_start_cond();                                          // Send start bit TO slave
//    MasterWaitForIntrI2C1();                                        // Wait for Master interrupt request and then clear interrupt Flag.
	
    switch(ucCmd)
    {
        case WRITE:
//			cmd = ucData_wr[0];
//			// Shift from one byte
//			for (i=0;i++;i<ucBytes_wr-1){
//				ucData_wr[i] = ucData_wr[i+1];
//			}
////			if(ret=i2c_smbus_read_i2c_block_data(SLAVE_FD, ucData_wr[1], usBytes_rd, ucData_rd)>0)
//			if(i2c_smbus_write_i2c_block_data(SLAVE_FD, cmd , ucBytes_wr, ucData_wr)<0){
//				printf("Error in i2c writing\n");         
//			}
            if (i2c_write_byte(TRUE,FALSE,SLAVE_ADDR)==ACK)                               // check for ACK from slave
            {
                for(i = 0; i < ucBytes_wr; i++)                     // Begin a loop writing the tx bytes to the slave
                {              
					i2c_write_byte(FALSE, FALSE, ucData_wr[i]); 
                }
            }
            
            else
                i2cIO_error(NOT_ACK);                              //ACK error (does not return)	   
            break;

        case READ:
		
			if (i2c_write_byte(TRUE,FALSE,SLAVE_ADDR | 1)==ACK)                               // check for ACK from slave
				{
					gets_I2C(ucData_rd, usBytes_rd, bAdjust);                             
				}
            else
                i2cIO_error(NOT_ACK);                               //ACK error (does not return)

            break;

        case WR_RD:			
//			cmd = ucData_wr[0];
//			for (i=0;i++;i<ucBytes_wr-1){
//				ucData_wr[i] = ucData_wr[i+1];
//			}
////			if(ret=i2c_smbus_read_i2c_block_data(SLAVE_FD, ucData_wr[1], usBytes_rd, ucData_rd)>0)
//			if(i2c_smbus_write_i2c_block_data(SLAVE_FD, cmd, ucBytes_wr-1, ucData_wr)<0){
//				printf("Error in i2c writing\n"); 
//				perror("Reason ");        
//			}
////			while(digitalRead(0) == 1);
//            gets_I2C(ucData_rd, usBytes_rd, bAdjust);              // Read in multiple bytes
//			if (i2c_write_byte(TRUE,FALSE,SLAVE_ADDR)==ACK)                               // check for ACK from slave
//            {
				i2c_write_byte(TRUE,FALSE,SLAVE_ADDR);
                for(i = 0; i < ucBytes_wr; i++)                     // Begin a loop writing the tx bytes to the slave
                {              
					i2c_write_byte(FALSE, FALSE, ucData_wr[i]); 
                }
//            }            
//            else
//                i2cIO_error(NOT_ACK);                              //ACK error (does not return)	   
                                   
			if (i2c_write_byte(TRUE,FALSE,SLAVE_ADDR | 1)==ACK)   // check for ACK from slave
				{
					gets_I2C(ucData_rd, usBytes_rd, bAdjust);                             
				}
            else
                i2cIO_error(NOT_ACK);                               //ACK error (does not return)
						
            break;
    }

	i2c_stop_cond();
	
    return I2C_SUCCESS;
}
