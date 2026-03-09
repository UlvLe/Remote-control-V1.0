#include "adc.h"
#include "cmsis_os2.h"

uint16_t adc1_values[4];

void ADC_Init(void){
    HAL_ADC_Start_DMA(&hadc1,(uint32_t*)adc1_values,4);  //开启ADC转换
}

uint16_t ADC_Get_Value(uint32_t index){
    return adc1_values[index];
}

uint8_t ReadJoystickbutton(void){
    uint8_t joystickbutton_state = 0;

    if(HAL_GPIO_ReadPin(L_JoystickButton_GPIO_Port,L_JoystickButton_Pin)==GPIO_PIN_SET){
        osDelay(10);
        if(HAL_GPIO_ReadPin(L_JoystickButton_GPIO_Port,L_JoystickButton_Pin)==GPIO_PIN_SET){
            joystickbutton_state = 1;
        }
    }
    if(HAL_GPIO_ReadPin(R_JoystickButton_GPIO_Port,R_JoystickButton_Pin)==GPIO_PIN_SET){
        osDelay(10);
        if(HAL_GPIO_ReadPin(R_JoystickButton_GPIO_Port,R_JoystickButton_Pin)==GPIO_PIN_SET){
            joystickbutton_state = 2;
        }
    }

    return joystickbutton_state;
}