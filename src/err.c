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
/** @file  err.c
*   Error handling for PIC24 with SSC7150
*****************************************************************************
*   MM7150 with Explorer 16 Development Board Sample Code error handling file
*
*   Company : Microchip Technology Inc.
*
*   File name : err.c
*
*   Summary : Module for MM7150 demo code which interfaces 
*           PIC24 (Explorer16 board) to SSC7150 via i2c bus/calls 
*   
*   Functions : error_handler
*               i2cIO_error
*
*   Revisions : 0.0 9-18-14 C21674 - initial version, enhanced error handling, added timer2 interrupt to exit for unresponsive i2c 
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

/** error_handler
* @note	Display error string LCD and to terminal emulator connected to UART2 on Explorer 16 board
* @param str function name string
* @param errval error value (in hex) returned to failing function
* @return 
*/
void error_handler( char *str, UINT8 offset, UINT8 errval)
{
    char err_buf[64];

    sprintf ( err_buf, "***Error in function: %s:%02X errval=0x%X*** \n\r", str, offset, errval);
	printf("%s", err_buf);

}



/** i2cIO_error
* @note	error return function, stops i2c, does not return 
* @param ucCode error code
* @return 
*/
void i2cIO_error (UINT8 ucCode)
{
    char err_buf[32];

    sprintf(err_buf, "***i2cIO_error code = 0x%X\n\r", ucCode);
	printf("%s", err_buf);
	
}   