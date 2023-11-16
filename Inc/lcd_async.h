#ifndef LCD_ASYNC_H
#define LCD_ASYNC_H

#include <stdint.h>
#include "stm32f4xx_hal_i2c.h"

#define LCD_ASYNC_TRANSMISSION_QUEUE_SIZE 4096

#define I2C_ADDR 0x27 // I2C address of the PCF8574
#define RS_BIT 0 // Register select bit
#define EN_BIT 2 // Enable bit
#define BL_BIT 3 // Backlight bit
#define D4_BIT 4 // Data 4 bit

struct LCD_TransmissionTask
{
    uint8_t data;
    uint8_t unsent;
};

struct LCD_TransmissionTask LCD_TRANSMISSION_QUEUE[LCD_ASYNC_TRANSMISSION_QUEUE_SIZE];
int LCD_TRANSMISSION_QUEUE_ADD_INDEX = 0;
int LCD_TRANSMISSION_QUEUE_EXE_INDEX = 0;

void __LCD_Transmit(uint8_t data)
{
    if (LCD_TRANSMISSION_QUEUE_ADD_INDEX >= LCD_ASYNC_TRANSMISSION_QUEUE_SIZE) {
        LCD_TRANSMISSION_QUEUE_ADD_INDEX = 0;
    }
    struct LCD_TransmissionTask *task = &LCD_TRANSMISSION_QUEUE[LCD_TRANSMISSION_QUEUE_ADD_INDEX];
    task->data = data;
    task->unsent = 1;
    LCD_TRANSMISSION_QUEUE_ADD_INDEX++;
}

void __LCD_WriteNibble(uint8_t nibble, uint8_t rs)
{
    uint8_t backlight_state = 1;
    uint8_t data = nibble << D4_BIT;
    data |= rs << RS_BIT;
    data |= backlight_state << BL_BIT; // Include backlight state in data
    data |= 1 << EN_BIT;
    __LCD_Transmit(data);
    data &= ~(1 << EN_BIT);
    __LCD_Transmit(data);
}

void __LCD_WriteNibbleBlocking(uint8_t nibble, uint8_t rs)
{
    uint8_t data = nibble << D4_BIT;
    data |= rs << RS_BIT;
    data |= 1 << BL_BIT; // Include backlight state in data
    data |= 1 << EN_BIT;
    HAL_I2C_Master_Transmit(&hi2c1, I2C_ADDR << 1, &data, 1, 1);
    HAL_Delay(1);
    data &= ~(1 << EN_BIT);
    HAL_I2C_Master_Transmit(&hi2c1, I2C_ADDR << 1, &data, 1, 1);
    HAL_Delay(1);
}

void __LCD_SendCmd(uint8_t cmd)
{
    uint8_t upper_nibble = cmd >> 4;
    uint8_t lower_nibble = cmd & 0x0F;
    __LCD_WriteNibble(upper_nibble, 0);
    __LCD_WriteNibble(lower_nibble, 0);
}


void __LCD_SendCmdBlocking(uint8_t cmd)
{
    uint8_t upper_nibble = cmd >> 4;
    uint8_t lower_nibble = cmd & 0x0F;
    __LCD_WriteNibbleBlocking(upper_nibble, 0);
    __LCD_WriteNibbleBlocking(lower_nibble, 0);
    if (cmd == 0x01 || cmd == 0x02) {
        HAL_Delay(2);
    }
};

void __LCD_SendData(uint8_t data)
{
    uint8_t upper_nibble = data >> 4;
    uint8_t lower_nibble = data & 0x0F;
    __LCD_WriteNibble(upper_nibble, 1);
    __LCD_WriteNibble(lower_nibble, 1);
}

void __LCD_SendDataBlocking(uint8_t data)
{
    uint8_t upper_nibble = data >> 4;
    uint8_t lower_nibble = data & 0x0F;
    __LCD_WriteNibbleBlocking(upper_nibble, 1);
    __LCD_WriteNibbleBlocking(lower_nibble, 1);
}

void LCD_Init()
{
    HAL_Delay(50);
    __LCD_WriteNibbleBlocking(0x03, 0);
    HAL_Delay(5);
    __LCD_WriteNibbleBlocking(0x03, 0);
    HAL_Delay(1);
    __LCD_WriteNibbleBlocking(0x03, 0);
    HAL_Delay(1);
    __LCD_WriteNibbleBlocking(0x02, 0);
    __LCD_SendCmdBlocking(0x28);
    __LCD_SendCmdBlocking(0x0C);
    __LCD_SendCmdBlocking(0x06);
    __LCD_SendCmdBlocking(0x01);
    HAL_Delay(2);
}

void LCD_TickQueue()
{
    if (LCD_TRANSMISSION_QUEUE_EXE_INDEX >= LCD_ASYNC_TRANSMISSION_QUEUE_SIZE) {
        LCD_TRANSMISSION_QUEUE_EXE_INDEX = 0;
    }
    struct LCD_TransmissionTask *task = &LCD_TRANSMISSION_QUEUE[LCD_TRANSMISSION_QUEUE_EXE_INDEX];
    if (task->unsent) {
        HAL_I2C_Master_Transmit(&hi2c1, I2C_ADDR << 1, &(task->data), 1, 10);
        task->unsent = 0;
    } else {
        struct LCD_TransmissionTask *nextTask = &LCD_TRANSMISSION_QUEUE[(LCD_TRANSMISSION_QUEUE_EXE_INDEX + 1) %
                                                                        LCD_ASYNC_TRANSMISSION_QUEUE_SIZE];
        if (nextTask->unsent) {
            LCD_TRANSMISSION_QUEUE_EXE_INDEX++;
        }
    }

}

void LCD_Clear(void)
{
    __LCD_SendCmd(0x01);
}

void LCD_ClearBlocking(void)
{
    __LCD_SendCmdBlocking(0x01);
}

void LCD_Print(char *str)
{
    while (*str) {
        __LCD_SendData(*str++);
    }
}

void LCD_PrintBlocking(char *str)
{
    while (*str) {
        __LCD_SendDataBlocking(*str++);
    }
}

void LCD_SetCursor(uint8_t x, uint8_t y)
{
    uint8_t address;
    switch (y) {
        case 0:
            address = 0x00;
            break;
        case 1:
            address = 0x40;
            break;
        default:
            address = 0x00;
    }
    address += x;
    __LCD_SendCmd(0x80 | address);
}

void LCD_SetCursorBlocking(uint8_t row, uint8_t column)
{
    uint8_t address;
    switch (row) {
        case 0:
            address = 0x00;
            break;
        case 1:
            address = 0x40;
            break;
        default:
            address = 0x00;
    }
    address += column;
    __LCD_SendCmdBlocking(0x80 | address);
}

#endif //LCD_ASYNC_H
