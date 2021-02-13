#include <avr/io.h>
#include <util/delay.h>


#include "iic.h"

uint32_t ms10_counter = 0;
uint32_t ms10_cur = 0;
uint32_t ms10_lastSignalLevel = 0;
uint32_t ms10_lastSignalSend = 0;
uint32_t ms10_lastRecieve = 0;
uint32_t ms10_lastRecieve0000 = 0;

bool bRecieveAudio = false;

uint8_t _aInitXeoDarr81[223] = {
    0x00, 0x90, 0x41, 0xE0, 0x00, 0xA4, 0x0B, 0x65, 0xE4, 0xAE, 0x22, 0x10, 0x3C, 0x61, 0x06, 0xE4,
    0x02, 0x79, 0x03, 0x65, 0xE4, 0xAE, 0x22, 0x50, 0x42, 0x24, 0x06, 0x65, 0x4A, 0xA3, 0xBC, 0x8C,
    0x14, 0xA9, 0x05, 0x8C, 0x12, 0xA3, 0x07, 0x2E, 0x02, 0x8C, 0x1F, 0x2D, 0x01, 0x3D, 0xBF, 0xA3,
    0x07, 0xAE, 0x22, 0x3D, 0xBF, 0xA3, 0x07, 0xAE, 0x22, 0x3D, 0xBF, 0xA3, 0x07, 0xAE, 0x22, 0x3D,
    0xBF, 0xA3, 0x07, 0xAE, 0x22, 0x3D, 0xBF, 0xA3, 0x07, 0xAE, 0x22, 0x3D, 0xBF, 0xA3, 0x07, 0xAE,
    0x22, 0x3D, 0xBF, 0xA3, 0x07, 0xAE, 0x22, 0x7D, 0x2B, 0x75, 0x9B, 0xAE, 0x22, 0xFD, 0x96, 0x36,
    0x48, 0x9F, 0xF0, 0xAE, 0x16, 0xF7, 0x04, 0xE6, 0x06, 0xE7, 0x26, 0x2F, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x4F, 0x42, 0x24, 0x06, 0x65, 0x07, 0x2C, 0x7B, 0x8C, 0x1D, 0xAD, 0x01, 0x3D, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x51, 0x42, 0x24, 0x06, 0x65, 0x07, 0xC7, 0x84, 0x6D, 0x12, 0x3D,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2C, 0x00
};
uint8_t _aInitRF[25] = {
    0xAA, 0x07, 0x55, 0x0E, 0x4B, 0x3E, 0xC8, 0x3C, 0xC8, 0xEC, 0x59, 0x01, 0x95, 0x06, 0xD1, 0x0B, 0x00, 0x00, 0x79, 0xD1, 0x7B, 0xEA, 0x76, 0x38, 0x55
   // 0xAA, 0x07, 0x55, 0x0E, 0x4B, 0x3E, 0xC8, 0x3C, 0xC8, 0xEC, 0x59, 0x01, 0x95, 0x06, 0x00, 0x00, 0x00, 0x00, 0x79, 0xD1, 0x7B, 0xDA, 0x76, 0x28, 0x55
};

uint8_t _aInitMCU[46][2] = {
      {0xC9, 0x01}, {0xCA, 0x02}, {0xCB, 0x01}, {0xC8, 0x25},
 //     {0x13, 0x7E}, {0x15, 0x01}, {0x16, 0x12}, {0x17, 0x12}, // 0x15 select transmitter
      {0x13, 0x7E}, {0x15, 0x01}, {0x16, 0x12}, {0x17, 0x12}, // 0x15 select transmitter
      {0x18, 0x3F}, {0x43, 0x00}, {0x1D, 0x00},// {0x1E, 0x7F},
      {0x1F, 0x05}, {0x2A, 0xA0}, {0x38, 0xA0}, {0x20, 0x70},
      {0x21, 0x70}, {0x22, 0x07}, {0x23, 0xE4}, {0x24, 0xE4},
    //  {0x29, 0x60}, {0x42, 0x21}, {0x44, 0xDF}, {0x45, 0xFF},
      {0x29, 0x60}, {0x42, 0x22}, {0x44, 0xDF}, {0x45, 0xFF},
      {0x43, 0x01}, 
      {0x4A, 0x3C}, {0x81, 0x07}, {0x82, 0x40}, {0x83, 0x85},
      {0x84, 0x37}, {0x8F, 0xA0}, {0x9F, 0x75}, {0x94, 0x31},
      {0x98, 0x31}, {0xD0, 0x40}, {0xD1, 0x17}, {0xDE, 0x28},
      {0xDF, 0x30}, {0xE4, 0xA3}, {0xE5, 0x0D}, {0xEC, 0x7F},
      {0xED, 0x00}, {0x1A, 0xD2}, {0x8C, 0x86}, {0x19, 0xBA},
      {0x10, 0x01},
     // {0x3B, 0xFF}, {0x3C, 0xFF}, {0x3D, 0x00}, {0x10, 0x01},
     // {0x13, 0x7F}, {0x14, 0x02}
};


uint8_t tempSendBuffer[32]; 
uint8_t tempRecvBuffer[32]; 

#define BLUE_LED_ON() {PORTB |= 0x01;} 
#define BLUE_LED_OFF() {PORTB &= ~0x01;} 

#define RED_LED_ON() {PORTB |= 0x02;} 
#define RED_LED_OFF() {PORTB &= ~0x02;} 

#define AUDIO_ON() {PORTC |= 0x02;} 
#define AUDIO_OFF() {PORTC &= ~0x02;} 

#define DARR_RESET_ON() {PORTC |= 0x08;} 
#define DARR_RESET_OFF() {PORTC &= ~0x08;} 

int GetSwitchState(){
  if ((PINB & 0xC0) == 0xC0) return 2;
  if ((PINB & 0x40) == 0x40) return 1;
  if ((PINB & 0x80) == 0x80) return 3;

  return 0;
}

int iSwitchState = 0;

uint8_t DT_Signal_Level[5];
uint8_t DT_Signal_Level_count = 0;
uint8_t DT_Signal_Level_avarage = 0;
uint8_t DT_Signal_Level_max = 0; 


uint8_t bReadByte(uint8_t command){
   tempSendBuffer[0] = command;
   __i2c_Write_Buf (0x80, tempSendBuffer, 1, 0); 
   __i2c_recv(1);
   return RX_Data_recv_array[0];
}

void Avarage_Signal_Level(){
  uint16_t uiAvarage = 0;
  for (uint8_t i=0; i<5; i++){
    uiAvarage += DT_Signal_Level[i];
  }
  uiAvarage -= DT_Signal_Level_max;
  uiAvarage /= 4;

  DT_Signal_Level_avarage = uiAvarage;
}

void Add_Signal_Level(){
   if (DT_Signal_Level_count <= 5){
     DT_Signal_Level[DT_Signal_Level_count] = bReadByte(0x74) & 0x7F;  
  
     if (DT_Signal_Level_count == 0){
        DT_Signal_Level_max = DT_Signal_Level[0];
     } else {
        DT_Signal_Level_max = ((DT_Signal_Level_max > DT_Signal_Level[DT_Signal_Level_count]) ? DT_Signal_Level_max : DT_Signal_Level[DT_Signal_Level_count]);
     }
   }
   DT_Signal_Level_count++;

   if (DT_Signal_Level_count >= 5){
      Avarage_Signal_Level();
     // DT_Signal_Level_count = 0;
   }
}

uint16_t bReadWord(uint16_t command){
   tempSendBuffer[0] = command;
   __i2c_Write_Buf (0x80, tempSendBuffer, 1, 0); 
   __i2c_recv(2);
   return (RX_Data_recv_array[0] << 8) + RX_Data_recv_array[0];
}

bool Send_Signal_Level(){  
  if (DT_Signal_Level_count < 5) return false;
  for (int i=0; i<17; i++) tempSendBuffer[i] = 0;
  tempSendBuffer[0] = 0x4B; tempSendBuffer[1] = 0x83;
  __i2c_Write_Buf (0x80, tempSendBuffer, 2, 0); 
  
  tempSendBuffer[0] = 0x50; tempSendBuffer[1] = 0x5D;
  tempSendBuffer[2] = DT_Signal_Level_avarage; tempSendBuffer[3] = DT_Signal_Level_avarage; 
  tempSendBuffer[4] = DT_Signal_Level_avarage; tempSendBuffer[5] = 0x7F;  

  uint16_t chk_sum = 0;
    
  for (int i=1; i<17; i++) chk_sum = chk_sum + tempSendBuffer[i];
  tempSendBuffer[16] = (chk_sum & 0xFF);
  
  __i2c_Write_Buf (0x80, tempSendBuffer, 17, 0); 
  DT_Signal_Level_count = 0;
  return true;
}

int main(void)
{       

  DDRB |= 0x01; // Blue led pin out mode
  DDRB |= 0x02; // Red led pin out mode

  DDRB &= ~0x40; // Switch pin 1
  DDRB &= ~0x80; // Switch pin 2
  
  DDRC |= 0x02;    // Audio out on
  DDRC |= 0x08;    // DARR81 reset


  AUDIO_OFF();
  

  DARR_RESET_OFF();
  _delay_ms(200);
  DARR_RESET_ON();
  _delay_ms(100);

  __i2c_init();
  RED_LED_ON()
  BLUE_LED_ON()

  __i2c_Write_Buf (0x82, _aInitXeoDarr81, 223, 0); 
  _delay_ms(10);
  __i2c_Write_Buf (0x80, _aInitRF, 25, 0); 
  _delay_ms(10);

  for (int i=0; i<46; i++){
    __i2c_Write_Buf (0x80, _aInitMCU[i], 2, 0); 
    _delay_ms(5);
  }

  _delay_ms(1000);

  
  TCCR0B = 0x05; // clock frequency / 1024 
  OCR0B = 0x00;  // Output compare
  TCNT0 = 0x9E; // Set counter 0 to zero
  TIMSK0 = 0x01; // Enable overflow interrupt

  sei();

//  ms10_counter = 0;
  ms10_cur = ms10_counter;
  ms10_lastSignalLevel = ms10_cur;
  ms10_lastSignalSend = ms10_cur;
  ms10_lastRecieve = ms10_cur;
  ms10_lastRecieve0000 = ms10_cur;


  
  BLUE_LED_OFF()
  RED_LED_ON()
  bool bWorkWithCenter = false;
  int iLostCounter = 0;

  while(1){
    while (ms10_cur == ms10_counter) _delay_ms(1);
    ms10_cur = ms10_counter;

    ShitchState(GetSwitchState());

    if (bWorkWithCenter){
      if ((ms10_cur - ms10_lastSignalLevel) > 7){
        Add_Signal_Level();
        ms10_lastSignalLevel = ms10_cur;
      }
    }

    if ((ms10_cur-ms10_lastRecieve0000)<14){
      _delay_ms(1);
      continue;
    }
    
    uint16_t get13 = bReadWord(0x13);
    if (get13 == 0x0000){      
      ms10_lastRecieve0000 = ms10_cur;
      _delay_ms(1);
      continue;
    }


    if ((ms10_cur - ms10_lastRecieve)>1000){
      RunAudio(false);
    }
    
    if ((get13 & 0x40) == 0x40){
      if (bWorkWithCenter){
        iLostCounter++;
        bReadByte(0x19);
        if (iLostCounter >= 5) {
          bWorkWithCenter = false;
          RunAudio(false);
          continue;
        }
      } else {
        iLostCounter = 0;
      }
      _delay_ms(250);
      continue;
    }
    iLostCounter = 0;
    bWorkWithCenter = true;
    
    if ((ms10_cur - ms10_lastSignalSend)>35){
      if (Send_Signal_Level())
        ms10_lastSignalSend = ms10_cur;
    }
    
    if (((get13 & 0x02) == 0x02) || ((ms10_cur - ms10_lastRecieve)>70)){
      _delay_ms(1);
      //uint8_t get4C = bReadByte(0x4C);
      while ((bReadByte(0x4C) & 0x04) != 0x04) {
        _delay_ms(1);
        uint8_t get4D = bReadByte(0x4D);
        _delay_ms(1);
        uint8_t get4E = bReadByte(0x4E);
        _delay_ms(1);  
        tempSendBuffer[0] = 0x50;
        __i2c_Write_Buf (0x80, tempSendBuffer, 1, 0); 
        __i2c_recv(16);

        if (RX_Data_recv_array[0] != 0xA0) continue;
        if (RX_Data_recv_array[1] != 0x90) continue;
        ms10_lastRecieve = ms10_cur;

        
        if ((RX_Data_recv_array[3] == 0x01)||(RX_Data_recv_array[4] == 0x01)){
          RunAudio(true);
        } else {
          RunAudio(false);
        }
      }
    }
  }
  return 0;
}

int iLastState = 0;
void ShitchState(int iState){
  if (iState != iLastState){
    if (iState == 1){
      tempSendBuffer[1] = 0x39;
    }
    if (iState == 2){
      tempSendBuffer[1] = 0x93;
    }
    if (iState == 3){
      tempSendBuffer[1] = 0xE4;
    }
    tempSendBuffer[0] = 0x23; 
    __i2c_Write_Buf (0x80, tempSendBuffer, 2, 0);
    tempSendBuffer[0] = 0x22; tempSendBuffer[1] = 0x0F;
    __i2c_Write_Buf (0x80, tempSendBuffer, 2, 0); 
    tempSendBuffer[0] = 0x21; tempSendBuffer[1] = 0xF0;
    __i2c_Write_Buf (0x80, tempSendBuffer, 2, 0); 
    
    iLastState = iState;
  }
}

void RunAudio(bool bRun){
  if (bRun){
    RED_LED_OFF()
    BLUE_LED_ON()
    AUDIO_ON()
  } else {
    RED_LED_ON()
    BLUE_LED_OFF()
    AUDIO_OFF()
  }
}

ISR(TIMER0_OVF_vect) //Timer 0 overflow vector - this run every time timer0 overflows
{
   ms10_counter++;    
   TCNT0 = 0x9D; // Set counter 0 to zero
}
