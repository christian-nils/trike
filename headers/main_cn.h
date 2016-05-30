#include <wiringPiI2C.h>
#include <iostream>
#include <errno.h>
#include <string.h>

// Custom headers
#include "i2cIO.h"

// Declare new types
typedef unsigned char UINT8;        // 8-bit variables
typedef unsigned char BOOL;
   
typedef unsigned short UINT16;      // 16-bit variables
typedef unsigned long UINT32;

// Define type
#define TRUE 1
#define FALSE 0

#define SLAVE_ADDR 0x40
extern int SLAVE_FD;

using namespace std;
