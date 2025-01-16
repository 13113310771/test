#ifndef _as5600_task_H
#define _as5600_task_H

#include "stm32f10x.h"
#include "i2c.h"

#define CONF_LO_PARAMETER 0x0//00  00 00 00 
#define CONF_HI_PARAMETER 0x20//00 1 000 00

 

/*更改所控制的AS5600个数，这里更改了则i2c.c中More_IIC_DATA_Read()函数也需要更改*/
#define AS5600_NUMBER 6


typedef struct
{
  uint8_t _zmco ; 
  uint8_t _zpos_hi ; 
  uint8_t _zpos_lo ; 
  uint8_t _mpos_hi ; 
  uint8_t _mpos_lo ; 
  uint8_t _mang_hi ; 
  uint8_t _mang_lo ; 
  uint8_t _conf_hi ;     
  uint8_t _conf_lo ; 
	uint8_t _stat ;
  uint8_t _raw_ang_hi ; 
  uint8_t _raw_ang_lo ; 
  uint8_t _ang_hi ; 
  uint8_t _ang_lo ; 
   
  uint8_t _agc ; 
  uint8_t _mag_hi ; 
  uint8_t _mag_lo ; 
  uint8_t _burn ; 
	
}AMS5600Registers_t;
typedef enum
{
/* set i2c address */ 
  _ams5600_Address = 0x36,
  _zmco = 0x00,
  _zpos_hi = 0x01,
  _zpos_lo = 0x02,
  _mpos_hi = 0x03,
  _mpos_lo = 0x04,
  _mang_hi = 0x05,
  _mang_lo = 0x06,
  _conf_hi = 0x07,    
  _conf_lo = 0x08,
	_stat = 0x0b,		//做为安全和诊断的特征
  _raw_ang_hi = 0x0c,
  _raw_ang_lo = 0x0d,
  _ang_hi = 0x0e,
  _ang_lo = 0x0f,
	_agc = 0x1a,		//显示温度补偿状态
  _mag_hi = 0x1b,
  _mag_lo = 0x1c,
  _burn = 0xff		//固定参数 0x80烧录zops与mpos 0x40烧录mang与config
}AMS5600Registers_adress;



void as5600_Init_task(void);
void AMS5600Registers_Read_task(void);
void as5600_Test_task(void);
void as5600_task(void);



extern uint16_t AS5600_Angle_16bit[AS5600_NUMBER];

#endif








