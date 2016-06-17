#include "../headers/app.h"

//void I2C_delay( void );
//BOOL read_SCL( void ); // Set SCL as input and return current level of line, 0 or 1
//BOOL read_SDA( void ); // Set SDA as input and return current level of line, 0 or 1
//void set_SCL( void ); // Actively drive SCL signal high
//void clear_SCL( void ); // Actively drive SCL signal low
//void set_SDA( void ); // Actively drive SDA signal high
//void clear_SDA( void ); // Actively drive SDA signal low
////void arbitration_lost( void ); not needed when only one device
extern clock_t POR_TIMER;											// amount of elapsed time (in ms) since POR 
BOOL started = FALSE; // global data

void init_I2C(void){
	
	UINT32 dPOR_TIMER = 0;
	clock_t tp;
	
	pinMode(SCLPIN, OUTPUT);
	pinMode(SDAPIN, OUTPUT);
	set_SCL();
	set_SDA();
	
	while (dPOR_TIMER < I2C_POR_TIMEOUT){							//wait here for 2 seconds elapsed since POR for MM7150 i2c engine to be up and running
		tp = clock();
		dPOR_TIMER = (UINT32) ((tp-POR_TIMER)/(double)CLOCKS_PER_SEC*1000);
 	}
}

BOOL read_SCL(void){
	pinMode(SCLPIN, INPUT);
	if (digitalRead(SCLPIN)){
		pinMode(SCLPIN, OUTPUT);
		return TRUE;
	}
	else {
		pinMode(SCLPIN, OUTPUT);
		return FALSE;
	}
}

void set_SCL( void ){ // Actively drive SCL signal high
	digitalWrite(SCLPIN, HIGH);
}
void clear_SCL( void ){ // Actively drive SCL signal low
	digitalWrite(SCLPIN, LOW);
}

BOOL read_SDA(void){
	pinMode(SDAPIN, INPUT);	
	if (digitalRead(SDAPIN)){
		pinMode(SDAPIN, OUTPUT);
		return TRUE;
	}
	else {
		pinMode(SDAPIN, OUTPUT);
		return FALSE;
	}
}

void set_SDA( void ){ // Actively drive SDA signal high
	digitalWrite(SDAPIN, HIGH);
}
void clear_SDA( void ){ // Actively drive SDA signal low
	digitalWrite(SDAPIN, LOW);
}

void i2c_start_cond( void ) 
{
	UINT32 dPOR_TIMER = 0;
	clock_t tp;
  if( started ) 
  { 
    // if started, do a restart cond
    // set SDA to 1
    set_SDA();
    I2C_delay();

    while( read_SCL() == 0 && dPOR_TIMER < I2C_POR_TIMEOUT/10) 
    {  // Clock stretching
      // You should add timeout to this loop
		tp = clock();
		dPOR_TIMER = (UINT32) ((tp-POR_TIMER)/(double)CLOCKS_PER_SEC*1000);
    }

    // Repeated start setup time, minimum 4.7us
    I2C_delay();

  }

//  if( read_SDA() == 0 ) 
//  {
//    arbitration_lost();
//  }

  // SCL is high, set SDA from 1 to 0.
  clear_SDA();
  I2C_delay();
  clear_SCL();
  started = TRUE;

}

void i2c_stop_cond( void )
{
	UINT32 dPOR_TIMER = 0;
	clock_t tp;
  // set SDA to 0
  clear_SDA();
  I2C_delay();

  // Clock stretching
  while( read_SCL() == 0 && dPOR_TIMER < I2C_POR_TIMEOUT/10) 
  {
    // add timeout to this loop.
	tp = clock();
	dPOR_TIMER = (UINT32) ((tp-POR_TIMER)/(double)CLOCKS_PER_SEC*1000);
  }

  // Stop bit setup time, minimum 4us
  I2C_delay();

  // SCL is high, set SDA from 0 to 1
  set_SDA();
  I2C_delay();

//  if( read_SDA() == 0 ) 
//  {
//    arbitration_lost();
//  }

  I2C_delay();
  set_SCL();
  started = FALSE;
}

// Write a bit to I2C bus
void i2c_write_bit( BOOL bit ) 
{
	UINT32 dPOR_TIMER = 0;
	clock_t tp;
	
  if( bit ) 
  {
    set_SDA();
  } 
  else 
  {
    clear_SDA();
  }

  // SDA change propagation delay
  I2C_delay();  

  // Set SCL high to indicate a new valid SDA value is available
  set_SCL();

  // Wait for SDA value to be read by slave, minimum of 4us for standard mode
  I2C_delay();

  while( read_SCL() == 0 && dPOR_TIMER < I2C_POR_TIMEOUT/10) 
  { // Clock stretching
    // You should add timeout to this loop
	tp = clock();
	dPOR_TIMER = (UINT32) ((tp-POR_TIMER)/(double)CLOCKS_PER_SEC*1000);
  }
  // SCL is high, now data is valid
  // If SDA is high, check that nobody else is driving SDA
//  if( bit && ( read_SDA() == 0 ) )
//  {
//    arbitration_lost();
//  }

  // Clear the SCL to low in preparation for next change
  clear_SCL();
}

// Read a bit from I2C bus
BOOL i2c_read_bit( void ) 
{
  BOOL bit;

  // Let the slave drive data
//  set_SDA();
  pinMode(SDAPIN, INPUT);
//  pullUpDnControl(SDAPIN, PUD_UP);
  // Wait for SDA value to be written by slave, minimum of 4us for standard mode
  I2C_delay();

  // Set SCL high to indicate a new valid SDA value is available
  set_SCL();

  while( read_SCL() == 0 ) 
  { // Clock stretching
    // You should add timeout to this loop
  }

  // Wait for SDA value to be read by slave, minimum of 4us for standard mode
  I2C_delay();

  // SCL is high, read out bit
  bit = read_SDA();

  // Set SCL low in preparation for next operation
  clear_SCL();

  return bit;

}

// Write a byte to I2C bus. Return 0 if ack by the slave.
BOOL i2c_write_byte( BOOL          send_start ,
                     BOOL          send_stop  ,
                     UINT8			byte) 
{
  unsigned bit;
  BOOL     nack;

  if( send_start ) 
  {
    i2c_start_cond();
  }
  for( bit = 0; bit < 8; bit++ ) 
  {
    i2c_write_bit( ( byte & 0x80 ) != 0 );
    byte <<= 1;
  }
  nack = i2c_read_bit();
  
  if (send_stop) 
  {
    i2c_stop_cond();
  }
  
  return nack;

}

// Read a byte from I2C bus
UINT8 i2c_read_byte( BOOL nack , BOOL send_stop ) 
{
  UINT8 byte = 0;
  unsigned char bit;

  for( bit = 0; bit < 8; bit++ ) 
  {
    byte = ( byte << 1 ) | i2c_read_bit();
  }

  i2c_write_bit( nack );

  if( send_stop ) 
  {
    i2c_stop_cond();
  }

  return byte;

}

void I2C_delay( void ) 
{ 
  volatile int v;
  int i;

  for( i = 0; i < I2CSPEED / 2; i++ )
  {
    v;
  }

}

UINT8 i2c_get_address(void)
{
	int i, j;
	
	for (i = 0; i < 128; i += 16) {
//		printf("%02x: ", i);
		for(j = 0; j < 16; j++) {
			/* Select detection command for this address */
			
				if (i+j >= 0x03 && i+j <= 0x77){		
					if (i2c_write_byte(TRUE,TRUE,i+j | 1)==ACK){
						printf("%02x: ", i+j);
					}					
				}

		}
	}
}