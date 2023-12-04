#include "main.h"
#include "tim.h"
#include "bsp_PMSM.h"

#define PMSMOTOR_TIM_PERIOD 5000
#define PWM_Duty 0.15

uint8_t HallGetPhase(void){
    uint8_t phase=0;
    phase|=HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_6);
    phase<<=1;
    phase|=HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_7);
    phase<<=1;
    phase|=HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_8);
    return(phase&0x0007);
}

void PMSMortorPhaseControl(uint8_t ctrl_hall_phase){
    switch (ctrl_hall_phase)
    {
    /* 定义电机的U(A),V(B),W(C)三相分别对应是CH1,CH2,CH3;
     *  A+,A-分别表示CH1控制的上,下桥臂导通
     */
    case 5: //B+  A-
    {
      HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);
      /*  Channe3 configuration */ 
      HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3);
      HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_3);
    
      /*  Channe2 configuration  */
      __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2,PMSMOTOR_TIM_PERIOD * PWM_Duty);
      HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
      HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_2);  
      
      /*  Channe1 configuration */
      __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1,PMSMOTOR_TIM_PERIOD +1);
      HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
      HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
    }
    break;
    
    case 4:// C+ A-
    {
      /*  Channe2 configuration */ 
      HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
      HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_2);
 
      /*  Channe3 configuration  */
      __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3,PMSMOTOR_TIM_PERIOD * PWM_Duty);
      HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
      HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_3);  
      
      /*  Channe1 configuration  */
      __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,PMSMOTOR_TIM_PERIOD +1);
      HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
      HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
    }
      break;
    
    case 6://C+ B-
    {
      /*  Channe1 configuration  */ 
      HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
      HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_1);
    
      /*  Channe3 configuration */
      __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,PMSMOTOR_TIM_PERIOD * PWM_Duty);
      HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
      HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_3);  

      /*  Channe2 configuration  */
      __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,PMSMOTOR_TIM_PERIOD +1);
      HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
      HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
    }
      break;

    case 2: // A+ B-
    {
      /*  Channe3 configuration */       
      HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3);
      HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_3);
    
      /*  Channe1 configuration */
      __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,PMSMOTOR_TIM_PERIOD * PWM_Duty);
      HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
      HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_1);  
      
      /*  Channe2 configuration */
      __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,PMSMOTOR_TIM_PERIOD +1);
      HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
      HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
    }
      break;
    
    
    case 3:// A+ C-
    {
      /*  Channe2 configuration */ 
      HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
      HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_2);

      /*  Channe1 configuration */
      __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,PMSMOTOR_TIM_PERIOD * PWM_Duty);
      HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
      HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_1);  
      
      /*  Channe3 configuration */
      __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,PMSMOTOR_TIM_PERIOD +1);    
      HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3);
      HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);
    }
      break;
    case 1: // B+ C-
    {
      /*  Channe1 configuration */ 
      HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
      HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_1);
    
      /*  Channe2 configuration */
      __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2, PMSMOTOR_TIM_PERIOD * PWM_Duty);
      HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
      HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_2);  
      
      /*  Channe3 configuration */
      __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, PMSMOTOR_TIM_PERIOD +1);
      HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3);
      HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);
    }
    break;
  }
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){
    uint8_t hall_phase=0;
    hall_phase=HallGetPhase();
    PMSMortorPhaseControl(hall_phase);
}

void PMSMotorStart(void){
    uint8_t hall_phase;

    __HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_1,0);
    __HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_2,0);
    __HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_3,0);
    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);
    HAL_TIMEx_PWMN_Start(&htim1,TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Start(&htim1,TIM_CHANNEL_2);
    HAL_TIMEx_PWMN_Start(&htim1,TIM_CHANNEL_3);
    HAL_TIM_GenerateEvent(&htim1,TIM_EGR_COMG);
    __HAL_TIM_CLEAR_FLAG(&htim1,TIM_FLAG_COM);

    HAL_Delay(10);

    hall_phase=HallGetPhase();
    PMSMortorPhaseControl(hall_phase);
    HAL_TIM_GenerateEvent(&htim1,TIM_EGR_COMG);
    __HAL_TIM_CLEAR_FLAG(&htim1,TIM_FLAG_COM);
}
