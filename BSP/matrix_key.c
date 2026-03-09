#include "main.h"
#include "cmsis_os2.h"

//激活行
void SetActiveRow(uint8_t row) {
    // 先全部设为高
    HAL_GPIO_WritePin(ROW0_GPIO_Port, ROW0_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(ROW1_GPIO_Port, ROW1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(ROW2_GPIO_Port, ROW2_Pin, GPIO_PIN_SET);

    // 激活指定行
    switch (row) {
        case 0:
            HAL_GPIO_WritePin(ROW0_GPIO_Port, ROW0_Pin, GPIO_PIN_RESET);
            break;
        case 1:
            HAL_GPIO_WritePin(ROW1_GPIO_Port, ROW1_Pin, GPIO_PIN_RESET);
            break;
        case 2:
            HAL_GPIO_WritePin(ROW2_GPIO_Port, ROW2_Pin, GPIO_PIN_RESET);
            break;
    }
}

// 读取某一列状态
GPIO_PinState ReadColumn(uint8_t col) {
    switch (col) {
        case 0: return HAL_GPIO_ReadPin(COL0_GPIO_Port, COL0_Pin);
        case 1: return HAL_GPIO_ReadPin(COL1_GPIO_Port, COL1_Pin);
        case 2: return HAL_GPIO_ReadPin(COL2_GPIO_Port, COL2_Pin);
        default: return GPIO_PIN_SET;
    }
}

// 扫描键盘
uint16_t ScanMatrixKey(void) {
    uint16_t key_state = 0;

    for (uint8_t row = 0; row < 3; row++) {
        SetActiveRow(row);
        osDelay(1); // 等待电平稳定

        for (uint8_t col = 0; col < 3; col++) {
            if (ReadColumn(col) == GPIO_PIN_RESET) { // 检测到低电平 → 按下
                osDelay(10); // 消抖
                if (ReadColumn(col) == GPIO_PIN_RESET) {
                    uint8_t key_index = row * 3 + col; // 按键编号 0~8
                    key_state |= (1U << key_index);
                } 
            }
        }
    }
    return key_state;
}