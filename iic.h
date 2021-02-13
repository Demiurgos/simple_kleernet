#ifndef ___ICC_H 
#define ___ICC_H 

#include <avr/io.h>
 
#define __SDA_PIN 0x10
#define __SCL_PIN 0x20

#define __i2c_PORT      PORTC
#define __i2c_DDR       DDRC 
#define __i2c_PIN       PINC 

#define I2C_RBUFFER_SIZE 40 
#define I2C_WBUFFER_SIZE 40 

extern unsigned char RX_Data_recv_array[I2C_RBUFFER_SIZE]; 
 
extern unsigned char i2cSlaveAddress; 
extern unsigned char Error_Code; 
 
void __i2c_init(void); 
 
unsigned char __i2c_start(void); 
 
unsigned char __i2c_stop(void); 
unsigned char __i2c_send_ex(unsigned char addr, unsigned char count, unsigned char send_stop); /*uses fixed buffer*/ 
unsigned char __i2c_sendbuf_ex(unsigned char addr, unsigned char *buf, unsigned char count, unsigned char send_stop); 
unsigned char __i2c_recv(unsigned char count); 
unsigned char __i2c_byte_out(unsigned char dat); 
 
unsigned char __i2c_Write_Byte(unsigned char map,unsigned char data); 
unsigned char __i2c_Read_Byte(unsigned char map); 
unsigned char __i2c_Read_Buf (unsigned char addr, unsigned char rx_count); 
unsigned char __i2c_Write_Buf (unsigned char addr, unsigned char *buf, unsigned char count, unsigned char flash_type); 
 
 

#define I2C_ERR_OK       0       /* No error */ 
#define I2C_ERR_NAK      1       /* No ACK from slave */ 
#define I2C_ERR_LOST     2       /* Arbitration lost */ 
#define I2C_ERR_BUS      3       /* Bus is stuck (not used yet) */ 
#define I2C_ERR_TIMEOUT  4       /* Timeout on bus */ 
 
#endif 
