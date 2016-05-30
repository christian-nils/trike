#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <iostream>
#include <errno.h>
#include <string.h>

// Declare new types
typedef unsigned char UINT8;        // 8-bit variables
typedef unsigned char BOOL;
   
typedef unsigned short UINT16;      // 16-bit variables
typedef unsigned long UINT32;

typedef struct HID_DESCRIPTOR_TABLE                                 // Struct to store HID descriptor fields
{
    UINT16 wHIDDescLen;                                             // MSB 1; LSB 0
    UINT16 wRepDescLen;                                             // MSB 5; LSB 4
    UINT16 wRepDescReg;                                             // MSB 6; LSB 7
    UINT16 wMaxInpLen;                                              // MSB 11; LSB 10
    UINT16 wCmdReg;                                                 // MSB 17; LSB 16
    UINT16 wDatReg;                                                 // MSB 19; LSB 18
    UINT16 wVenID;                                                  // MSB 21; LSB 20
    UINT16 wProdID;                                                 // MSB 23; LSB 22
    UINT16 wVerID;                                                  // MSB 25; LSB 24
} HID_DESCRIPTOR_TABLE;

// Define type
#define TRUE 1
#define FALSE 0

#define CMD_LEN                 0x04
#define POWER_CMD_MSB           0x08                                // POWER command bytes
#define POWER_CMD_LSB           0x00
#define BUF_40                  40                                  //temp buffer size = 40 bytes
#define BYTE_SHIFT              8

#define SLAVE_ADDR 0x40
extern int SLAVE_FD;

// Custom headers
#include "i2cIO.h"

using namespace std;
