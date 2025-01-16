#ifndef _model_task_H
#define _model_task_H

#include "stm32f10x.h"                  // Device header

#include "as5600_task.h"



#define MODEL_X_MAX 771
#define MODEL_Y_MAX 766

extern int16_t model_data[AS5600_NUMBER];
extern uint8_t controller_mode;
void conformity_init(void);
void model_task(void);

#endif 
