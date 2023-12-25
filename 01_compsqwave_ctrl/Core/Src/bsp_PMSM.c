#include "main.h"
#include "tim.h"
#include "bsp_PMSM.h"

uint8_t HallGetPhase(void){
    uint8_t phase=0;
    phase|=HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_6);
    phase<<=1;
    phase|=HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_7);
    phase<<=1;
    phase|=HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_8);
    return(phase&0x0007);
}

void PMSMotor_TIM_CHx_Mode(TIM_HandleTypeDef *htim,uint32_t OutputMode,uint32_t CHx){
  uint32_t tmpccmrx=0;
  TIM_TypeDef *TIMx=htim->Instance;
  switch (CHx)
  {
  case TIM_CHANNEL_1:
    tmpccmrx=TIMx->CCMR1;
    tmpccmrx &= ~TIM_CCMR1_OC1M;
    tmpccmrx &= ~TIM_CCMR1_CC1S;
    tmpccmrx |= OutputMode;
    TIMx->CCMR1=tmpccmrx;
    break;
  case TIM_CHANNEL_2:
    tmpccmrx=TIMx->CCMR1;
    tmpccmrx &= ~TIM_CCMR1_OC2M;
    tmpccmrx &= ~TIM_CCMR1_CC2S;
    tmpccmrx |= (OutputMode<<8);
    TIMx->CCMR2=tmpccmrx;
    break;
  case TIM_CHANNEL_3:
    tmpccmrx=TIMx->CCMR2;
    tmpccmrx &= ~TIM_CCMR2_OC3M;
    tmpccmrx &= ~TIM_CCMR2_CC3S;
    tmpccmrx |= OutputMode;
    TIMx->CCMR1=tmpccmrx;
    break;
  default:
    break;
  }
}

void PMSMortorPhaseControl(uint8_t ctrl_hall_phase){
  switch (ctrl_hall_phase)
  {
  /* 定义电机的U(A),V(B),W(C)三相分别对应是CH1,CH2,CH3;
    *  A+,A-分别表示CH1控制的上,下桥臂导通
    */
  case 5: //V+ U-
    /*  Channe3 configuration */ 
    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3);
    HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_3);
  
    /*  Channe2 configuration  */
    /* V+ PA9 */
    //OCMODE_PWM1为未达CCR则电平有效
    PMSMotor_TIM_CHx_Mode(&htim1,TIM_OCMODE_PWM1,TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
    HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);  
    
    /*  Channe1 configuration */
    /* U- PB13 */
    //OCMODE_PWM1为未达CCR则电平无效
    PMSMotor_TIM_CHx_Mode(&htim1,TIM_OCMODE_PWM2,TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
    break;
  
  case 4://W+ U-
    /*  Channe2 configuration */ 
    /* V+ */
    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
    HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_2);

    /*  Channe3 configuration  */
    /* W+ PA10 */
    PMSMotor_TIM_CHx_Mode(&htim1,TIM_OCMODE_PWM1,TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
    HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);  
    
    /*  Channe1 configuration  */
    /* U- PB13 */
    PMSMotor_TIM_CHx_Mode(&htim1,TIM_OCMODE_PWM2,TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
    break;
  
  case 6://W+ V-
    /*  Channe1 configuration  */ 
    /* U- */
    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_1);
  
    /*  Channe3 configuration */
    /* W+ PA10 */
    PMSMotor_TIM_CHx_Mode(&htim1,TIM_OCMODE_PWM1,TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
    HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);  

    /*  Channe2 configuration  */
    /* V- PB14 */
    PMSMotor_TIM_CHx_Mode(&htim1,TIM_OCMODE_PWM2,TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
    HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
    break;

  case 2: // U+ V-
    /*  Channe3 configuration */
    /* W+ */      
    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3);
    HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_3);
  
    /*  Channe1 configuration */
    /* U+ PA8 */
    PMSMotor_TIM_CHx_Mode(&htim1,TIM_OCMODE_PWM1,TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);  
    
    /*  Channe2 configuration */
    /* V- PB14 */
    PMSMotor_TIM_CHx_Mode(&htim1,TIM_OCMODE_PWM2,TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
    HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
    break;
  
  
  case 3:// U+ W-
    /*  Channe2 configuration */
    /* V- */
    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
    HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_2);

    /*  Channe1 configuration */
    PMSMotor_TIM_CHx_Mode(&htim1,TIM_OCMODE_PWM1,TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);  
    
    /*  Channe3 configuration */
    PMSMotor_TIM_CHx_Mode(&htim1,TIM_OCMODE_PWM2,TIM_CHANNEL_3);   
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
    HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);
    break;
  case 1: // V+ W-
    /*  Channe1 configuration */ 
    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_1);
  
    /*  Channe2 configuration */
    PMSMotor_TIM_CHx_Mode(&htim1,TIM_OCMODE_PWM1,TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
    HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);  
    
    /*  Channe3 configuration */
    PMSMotor_TIM_CHx_Mode(&htim1,TIM_OCMODE_PWM2,TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
    HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);
    break;
  default:
    break;
  }
}

void PMSMotorStart(void){
  __IO uint8_t hall_phase=0;
  PMSMotor_TIM_CHx_Mode(&htim1,TIM_OCMODE_FORCED_ACTIVE,TIM_CHANNEL_1);
  PMSMotor_TIM_CHx_Mode(&htim1,TIM_OCMODE_FORCED_ACTIVE,TIM_CHANNEL_2);
  PMSMotor_TIM_CHx_Mode(&htim1,TIM_OCMODE_FORCED_ACTIVE,TIM_CHANNEL_3);

  HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_1);
  HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_2);
  HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_3);
  HAL_TIMEx_PWMN_Start(&htim1,TIM_CHANNEL_1);
  HAL_TIMEx_PWMN_Start(&htim1,TIM_CHANNEL_2);
  HAL_TIMEx_PWMN_Start(&htim1,TIM_CHANNEL_3);
  HAL_TIM_GenerateEvent(&htim1,TIM_EVENTSOURCE_COM);

  HAL_Delay(10);

  hall_phase=HallGetPhase();
  PMSMortorPhaseControl(hall_phase);
  HAL_TIM_GenerateEvent(&htim1,TIM_EVENTSOURCE_COM);
  __HAL_TIM_CLEAR_FLAG(&htim1,TIM_FLAG_COM);
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){
  __IO uint8_t hall_phase=0;
  hall_phase=HallGetPhase();
  PMSMortorPhaseControl(hall_phase);
}
