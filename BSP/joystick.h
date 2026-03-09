#ifndef JOYSTICK_H
#define JOYSTICK_H

extern uint16_t adc1_values[4];

void ADC_Init(void);
uint16_t ADC_Get_Value(uint32_t index);
uint8_t ReadJoystickbutton(void);

#endif