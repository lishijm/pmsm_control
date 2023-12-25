#ifndef __BSP_PMSM__
#define __BSP_PMSM__

#define PMSMOTOR_TIM_PERIOD 5000

#define PWM_DUTY 0.59f

#define LED1_TOGGLE HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);

void PMSMotorStart(void);

#endif
