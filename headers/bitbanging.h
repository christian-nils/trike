#define I2CSPEED 3000
#define SCLPIN 9 // pins number following wiringpi numbering
#define SDAPIN 8
#define ACK 0
#define NACK 1

void init_I2C(void);
BOOL read_SCL(void);
void set_SCL( void );
void clear_SCL( void );
BOOL read_SDA(void);
void set_SDA( void );
void clear_SDA( void );
void i2c_start_cond( void ) ;
void i2c_stop_cond( void );
void i2c_write_bit(BOOL bit) ;
BOOL i2c_read_bit( void ) ;
BOOL i2c_write_byte(BOOL send_start, BOOL send_stop, UINT8 byte) ;
UINT8 i2c_read_byte(BOOL nack , BOOL send_stop) ;
void I2C_delay( void ) ;
UINT8 i2c_get_address(void);