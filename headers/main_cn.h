#include <wiringPiI2C.h>
#include <iostream>
#include <errno.h>
#include <string.h>

#define SLAVE_ADDR 0x40
extern int SLAVE_FD;

using namespace std;
