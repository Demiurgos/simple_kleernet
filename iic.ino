
#include "iic.h"
  
  
unsigned char RX_Data_recv_array[I2C_RBUFFER_SIZE];  
 
#define __i2c_half_delay()    
#define __i2c_delay()  {_delay_us(2);};      
   
#define __SCL_H()         {__i2c_DDR = __i2c_DDR & ~__SCL_PIN;   __i2c_PORT = __i2c_PORT & ~__SCL_PIN;}  
#define __SCL_L()         {__i2c_DDR = __i2c_DDR | __SCL_PIN;   __i2c_PORT = __i2c_PORT & ~__SCL_PIN;} 
   
#define __SDA_H()         {__i2c_DDR = __i2c_DDR & ~__SDA_PIN;   __i2c_PORT = __i2c_PORT & ~__SDA_PIN;}  
#define __SDA_L()         {__i2c_DDR = __i2c_DDR | __SDA_PIN;   __i2c_PORT = __i2c_PORT & ~__SDA_PIN;}  


unsigned char i2cSlaveAddress = 0x80; 
 
unsigned char __SDA_IN(void)   
{   
    if ((__i2c_PIN & __SDA_PIN) == 0x00) return(0);   
    else return(1);   
}   
  
void __SDA_OUT(unsigned char b)   
{   
    if (b == 0) {__SDA_L();}   
    else {__SDA_H()}   
}   
   
unsigned char i2c_error = 0;            

void __i2c_init(void)   
{   
    __SCL_H();   
    __SDA_H();   
    i2c_error = 0;   
}   
   
   
   
unsigned char __i2c_start(void)   
{   
    __SCL_H();    
    __i2c_half_delay();   
    __SDA_L();   
    __i2c_half_delay();   
    __SCL_L();   
    __i2c_half_delay();   
    return 0;   
}   
   
unsigned char __i2c_stop(void)   
{   
    __SDA_L();   
    __i2c_half_delay();   
    __SCL_H();   
    __i2c_half_delay();   
    __SDA_H();   
    __i2c_half_delay();   
    return (0);   
}   
   
unsigned char __i2c_bit_out(unsigned char bout)   
{   
    int i = 0;   
    __SDA_OUT(bout);   
    __i2c_half_delay();   
    __SCL_H();     
    __i2c_delay();   
    __i2c_delay();   
    while (i != 1) i++; 
    __SCL_L();    
    __i2c_delay();   
    return 0;      
}   
   
   
unsigned char __i2c_bit_in(void)   
{   
    unsigned char bin;   
    int i = 0;   
    __SDA_H();   
    __i2c_half_delay();   
    __SCL_H();     
    __i2c_delay();   
    bin = __SDA_IN(); 
    while (i != 1) i++;
    __i2c_delay();   
    __SCL_L();    
    __i2c_delay();   
    return bin;     
}   

unsigned char __i2c_byte_out(unsigned char dat)   
{   
    unsigned char  bit_count;   
    bit_count = 8;   
    while(bit_count)   
    {   
        if (dat & 0x80)   
        {   
            if (__i2c_bit_out(1)) return 1;   
        }   
        else   
        {   
            if (__i2c_bit_out(0)) return 1;   
        }   
        dat <<= 1;   
        bit_count--;   
    }   
    if (__i2c_bit_in())   
    {   
        i2c_error = I2C_ERR_NAK;   
        return 1;   
    }   
    return 0;   
}   
   
    
unsigned char __i2c_byte_in(unsigned char ack)   
{   
    unsigned char bit_count, byte_in;   
   
    bit_count = 8;   
    byte_in = 0;   
   
    while(bit_count)   
    {   
        byte_in <<= 1;   
        if (__i2c_bit_in()) byte_in |= 0x01;   
        bit_count--;   
    }   
   
    __i2c_bit_out(ack);   
    __SDA_H();   
   
    return byte_in;   
}   
   
unsigned char __i2c_Write_Byte(unsigned char map,   unsigned char data)   
{   
    return (__i2c_Write_Buf(map, &data, 1,  0));   
}   
unsigned char __i2c_Read_Byte(unsigned char map)   
{   
    unsigned char tmp;   
    unsigned char result;   
             
    tmp = RX_Data_recv_array[0];   
       
    __i2c_Read_Buf(map, 1);   
       
    result = RX_Data_recv_array[0];   
    RX_Data_recv_array[0] = tmp;   
   
    return (result);   
}   
   
unsigned char __i2c_Read_Buf (unsigned char map, unsigned char rx_count)   
{   
   
    if ( __i2c_Write_Buf (map, &map, 0, 0))
    {   
   
        if (i2c_error != I2C_ERR_LOST) __i2c_stop();   
        return 1;   
    }   
   
    __SDA_H(); 
    __SCL_H();  
    __i2c_delay(); 
   
    if (__i2c_recv(rx_count)) return 1;   
   
    return (i2c_error ? 1 : 0);   
}   
   
unsigned char __i2c_recv(unsigned char count)   
{   
    unsigned char byteptr, byte_in;   
   
    if (__i2c_start()) return 1;   
    i2c_error = 0;   
    byteptr = 0;   
   
    byte_in = i2cSlaveAddress  | 0x01;   
   
    if (__i2c_byte_out(byte_in))   
    {   
   
        if (i2c_error == I2C_ERR_NAK) __i2c_stop();   
        return i2c_error;   
    }   
   
    while(count)   
    {   
        if (--count)   
        {   
            byte_in = __i2c_byte_in(0);   
        }   
        else   
        {   
            byte_in = __i2c_byte_in(1);   /* No ACK during last byte */   
        }   
        RX_Data_recv_array[byteptr] = byte_in;   
        byteptr++;   
    }   
   
    __i2c_stop();   
   
    return (i2c_error ? 1 : 0);   
}   

unsigned char __i2c_Write_Buf (unsigned char addr, unsigned char *buf, unsigned char count, unsigned char flash_type)   
   
{   
    unsigned char byte_out;   
   
    if (__i2c_start()) return 1;   
    i2c_error = 0;   
   
    byte_out = addr & 0xFE;   
   
    if (__i2c_byte_out(byte_out))   
    {   
        if (i2c_error == I2C_ERR_NAK )   
        {   
   
            __i2c_stop();   
            //Sync_Led_Status = LED_INIT_ERROR;   
            //Error_Code = 4;   
            //Error_Code_Display(Error_Code);   
               
        }   
        return i2c_error;   
    }          
   
    byte_out = *buf++;          
   
    while(count)   
    {   
           
        if (__i2c_byte_out(byte_out))   
        {   
            if (i2c_error == I2C_ERR_NAK)   
            {   
   
                __i2c_stop();   
            }   
            return i2c_error;   
        }   
   
        byte_out = *buf++;   
        count--;   
    }   
   
    __i2c_stop();   
    return 0;   
}   
