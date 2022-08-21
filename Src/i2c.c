/*
  i2c.c - I2C support for EEPROM, keypad and Trinamic plugins

  Part of grblHAL driver for STM32F4xx

  Copyright (c) 2018-2021 Terje Io

  Grbl is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Grbl is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Grbl.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <main.h>

#include "i2c.h"
#include "grbl/hal.h"

#if KEYPAD_ENABLE == 1
#include "keypad/keypad.h"
#endif

#if I2C_ENABLE

#ifdef FMP_I2C
  #define I2C1_SCL_PIN 6
  #define I2C1_SDA_PIN 7
#elif I2C1_ALT_PINMAP
  #define I2C1_SCL_PIN 6
  #define I2C1_SDA_PIN 7
#else
  #define I2C1_SCL_PIN 8
  #define I2C1_SDA_PIN 9
#endif

#ifdef FMP_I2C

#else
#define I2Cport(p) I2CportI(p)
#define I2CportI(p) I2C ## p
#endif

#ifdef FMP_I2C

#else
#define I2CPORT I2Cport(I2C_PORT)
#endif

#ifdef FMP_I2C
static FMPI2C_HandleTypeDef i2c_port = {
    .Instance = FMPI2C1,
    //.Init.Timing = 0xC0000E12, //400 KHz
    .Init.Timing = 0x0020081B, //1000 KHz
    //.Init.Timing =0x00401650, //200 KHz
    //.Init.DutyCycle = I2C_DUTYCYCLE_2,
    .Init.OwnAddress1 = 0,
    .Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT,
    .Init.DualAddressMode = I2C_DUALADDRESS_DISABLE,
    .Init.OwnAddress2 = 0,
    .Init.OwnAddress2Masks = FMPI2C_OA2_NOMASK,
    .Init.GeneralCallMode = I2C_GENERALCALL_DISABLE,
    .Init.NoStretchMode = I2C_NOSTRETCH_DISABLE
};
#else
static I2C_HandleTypeDef i2c_port = {
    .Instance = I2CPORT,
    .Init.ClockSpeed = 100000,
    .Init.DutyCycle = I2C_DUTYCYCLE_2,
    .Init.OwnAddress1 = 0,
    .Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT,
    .Init.DualAddressMode = I2C_DUALADDRESS_DISABLE,
    .Init.OwnAddress2 = 0,
    .Init.GeneralCallMode = I2C_GENERALCALL_DISABLE,
    .Init.NoStretchMode = I2C_NOSTRETCH_DISABLE
};
#endif

void i2c_init (void)
{
#if I2C_PORT == 1
    GPIO_InitTypeDef GPIO_InitStruct = {
        .Pin = (1 << I2C1_SCL_PIN)|(1 << I2C1_SDA_PIN),
        .Mode = GPIO_MODE_AF_OD,
        .Pull = GPIO_PULLUP,
        .Speed = GPIO_SPEED_FREQ_VERY_HIGH,
        .Alternate = GPIO_AF4_I2C1
    };
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    __HAL_RCC_I2C1_CLK_ENABLE();

    HAL_I2C_Init(&i2c_port);

    HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
    HAL_NVIC_EnableIRQ(I2C1_ER_IRQn);

    static const periph_pin_t scl = {
        .function = Output_SCK,
        .group = PinGroup_I2C,
        .port = GPIOB,
        .pin = I2C1_SCL_PIN,
        .mode = { .mask = PINMODE_OD }
    };

    static const periph_pin_t sda = {
        .function = Bidirectional_SDA,
        .group = PinGroup_I2C,
        .port = GPIOB,
        .pin = I2C1_SDA_PIN,
        .mode = { .mask = PINMODE_OD }
    };
#endif

#if I2C_PORT == 2
    GPIO_InitTypeDef GPIO_InitStruct = {
        .Pin = GPIO_PIN_10|GPIO_PIN_11,
        .Mode = GPIO_MODE_AF_OD,
        .Pull = GPIO_PULLUP,
        .Speed = GPIO_SPEED_FREQ_VERY_HIGH,
        .Alternate = GPIO_AF4_I2C2
    };
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    __HAL_RCC_I2C2_CLK_ENABLE();

    HAL_I2C_Init(&i2c_port);

    HAL_NVIC_EnableIRQ(I2C2_EV_IRQn);
    HAL_NVIC_EnableIRQ(I2C2_ER_IRQn);

    static const periph_pin_t scl = {
        .function = Output_SCK,
        .group = PinGroup_I2C,
        .port = GPIOB,
        .pin = 10,
        .mode = { .mask = PINMODE_OD }
    };

    static const periph_pin_t sda = {
        .function = Bidirectional_SDA,
        .group = PinGroup_I2C,
        .port = GPIOB,
        .pin = 11,
        .mode = { .mask = PINMODE_OD }
    };
#endif

#if FMP_I2C

    GPIO_InitTypeDef GPIO_InitStruct = {
        .Pin = (1 << I2C1_SCL_PIN)|(1 << I2C1_SDA_PIN),
        .Mode = GPIO_MODE_AF_OD,
        .Pull = GPIO_PULLUP,
        .Speed = GPIO_SPEED_FREQ_VERY_HIGH,
        .Alternate = GPIO_AF4_FMPI2C1
    };
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    __HAL_RCC_FMPI2C1_CLK_ENABLE();
    HAL_FMPI2CEx_ConfigAnalogFilter(&i2c_port, FMPI2C_ANALOGFILTER_ENABLE);

    HAL_FMPI2C_Init(&i2c_port);
    __HAL_FMPI2C_ENABLE(&i2c_port);

        /* FMPI2C1 interrupt Init */
    HAL_NVIC_SetPriority(FMPI2C1_EV_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(FMPI2C1_EV_IRQn);

    static const periph_pin_t scl = {
        .function = Output_SCK,
        .group = PinGroup_I2C,
        .port = GPIOC,
        .pin = 6,
        .mode = { .mask = PINMODE_OD }
    };

    static const periph_pin_t sda = {
        .function = Bidirectional_SDA,
        .group = PinGroup_I2C,
        .port = GPIOC,
        .pin = 7,
        .mode = { .mask = PINMODE_OD }
    };
#endif

    hal.periph_port.register_pin(&scl);
    hal.periph_port.register_pin(&sda);
}

#if I2C_PORT == 1
void I2C1_EV_IRQHandler(void)
{
  HAL_I2C_EV_IRQHandler(&i2c_port);
}

void I2C1_ER_IRQHandler(void)
{
  HAL_I2C_ER_IRQHandler(&i2c_port);
}
#elif FMP_I2C
void FMPI2C1_EV_IRQHandler(void)
{
  HAL_FMPI2C_EV_IRQHandler(&i2c_port);
}

void FMPI2C1_ER_IRQHandler(void)
{
  HAL_FMPI2C_ER_IRQHandler(&i2c_port);
}
#else
void I2C2_EV_IRQHandler(void)
{
  HAL_I2C_EV_IRQHandler(&i2c_port);
}

void I2C2_ER_IRQHandler(void)
{
  HAL_I2C_ER_IRQHandler(&i2c_port);
}
#endif

#ifdef FMP_I2C
void I2C_Send (uint32_t i2cAddr, uint8_t *buf, uint16_t bytes, bool block)
{
	//wait for bus to be ready
	while (HAL_FMPI2C_GetState(&i2c_port) != HAL_FMPI2C_STATE_READY);

    HAL_FMPI2C_Master_Transmit_IT(&i2c_port,  i2cAddr<<1, buf, bytes);

    if (block)
    	while (HAL_FMPI2C_GetState(&i2c_port) != HAL_FMPI2C_STATE_READY);
}
#else
void I2C_Send (uint32_t i2cAddr, uint8_t *buf, uint16_t bytes, bool block)
{
	//wait for bus to be ready
	while (HAL_I2C_GetState(&i2c_port) != HAL_I2C_STATE_READY);
	//need to replace this Pico function call

    HAL_I2C_Master_Transmit_DMA(&i2c_port,  i2cAddr, buf, bytes);

    if (block)
    	while (HAL_I2C_GetState(&i2c_port) != HAL_I2C_STATE_READY);
}
#endif

#if EEPROM_ENABLE
#ifdef FMP_I2C
nvs_transfer_result_t i2c_nvs_transfer (nvs_transfer_t *i2c, bool read)
{
    while (HAL_FMPI2C_GetState(&i2c_port) != HAL_FMPI2C_STATE_READY);

    HAL_StatusTypeDef ret;
    
    if(read)
        ret = HAL_FMPI2C_Mem_Read(&i2c_port, i2c->address << 1, i2c->word_addr, i2c->word_addr_bytes, i2c->data, i2c->count, 100);
    else {
        ret = HAL_FMPI2C_Mem_Write(&i2c_port, i2c->address << 1, i2c->word_addr, i2c->word_addr_bytes, i2c->data, i2c->count, 100);
#if !EEPROM_IS_FRAM
        hal.delay_ms(6, NULL);
#endif
    }
    i2c->data += i2c->count;

    return ret == HAL_OK ? NVS_TransferResult_OK : NVS_TransferResult_Failed;
}
#else
nvs_transfer_result_t i2c_nvs_transfer (nvs_transfer_t *i2c, bool read)
{
    while (HAL_I2C_GetState(&i2c_port) != HAL_I2C_STATE_READY);

    HAL_StatusTypeDef ret;

    if(read)
        ret = HAL_I2C_Mem_Read(&i2c_port, i2c->address << 1, i2c->word_addr, i2c->word_addr_bytes == 2 ? I2C_MEMADD_SIZE_16BIT : I2C_MEMADD_SIZE_8BIT, i2c->data, i2c->count, 100);
    else {
        ret = HAL_I2C_Mem_Write(&i2c_port, i2c->address << 1, i2c->word_addr, i2c->word_addr_bytes == 2 ? I2C_MEMADD_SIZE_16BIT : I2C_MEMADD_SIZE_8BIT, i2c->data, i2c->count, 100);
#if !EEPROM_IS_FRAM
        hal.delay_ms(5, NULL);
#endif
    }
    i2c->data += i2c->count;

    return ret == HAL_OK ? NVS_TransferResult_OK : NVS_TransferResult_Failed;
}
#endif //FMP_I2C
#endif // EEPROM_ENABLE

#if KEYPAD_ENABLE == 1

static uint8_t keycode = 0;
static keycode_callback_ptr keypad_callback = NULL;

void I2C_PendantRead (uint32_t i2cAddr, uint8_t memaddress, uint8_t size, uint8_t * data)
{
    while (HAL_FMPI2C_GetState(&i2c_port) != HAL_FMPI2C_STATE_READY);
    
    HAL_FMPI2C_Mem_Read(&i2c_port, i2cAddr << 1, memaddress, 1, data, size, 100);
}

#ifdef FMP_I2C
void HAL_FMPI2C_MasterRxCpltCallback(FMPI2C_HandleTypeDef *hi2c)
{
    if(keypad_callback && keycode != 0) {
        keypad_callback(keycode);
        keypad_callback = NULL;
    }
}
#else
void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if(keypad_callback && keycode != 0) {
        keypad_callback(keycode);
        keypad_callback = NULL;
    }
}
#endif

void I2C_GetKeycode (uint32_t i2cAddr, keycode_callback_ptr callback)
{
    keycode = 0;
    keypad_callback = callback;

    #ifdef FMP_I2C
    HAL_FMPI2C_Master_Receive_IT(&i2c_port, i2cAddr << 1, &keycode, 1);
    #else
     HAL_I2C_Master_Receive_IT(&i2c_port, KEYPAD_I2CADDR << 1, &keycode, 1);
    #endif
}

#endif // KEYPAD_ENABLE == 1

#if TRINAMIC_ENABLE && TRINAMIC_I2C

static uint16_t axis = 0xFF;
static const uint8_t tmc_addr = I2C_ADR_I2CBRIDGE << 1;

TMC_spi_status_t tmc_spi_read (trinamic_motor_t driver, TMC_spi_datagram_t *reg)
{
    uint8_t buffer[5] = {0};
    TMC_spi_status_t status = 0;

    if(driver.axis != axis) {
        buffer[0] = driver.axis | 0x80;
        HAL_I2C_Mem_Write(&i2c_port, tmc_addr, axis, I2C_MEMADD_SIZE_8BIT, buffer, 1, 100);

        axis = driver.axis;
    }

    HAL_I2C_Mem_Read(&i2c_port, tmc_addr, (uint16_t)reg->addr.idx, I2C_MEMADD_SIZE_8BIT, buffer, 5, 100);

    status = buffer[0];
    reg->payload.value = buffer[4];
    reg->payload.value |= buffer[3] << 8;
    reg->payload.value |= buffer[2] << 16;
    reg->payload.value |= buffer[1] << 24;

    return status;
}

TMC_spi_status_t tmc_spi_write (trinamic_motor_t driver, TMC_spi_datagram_t *reg)
{
    uint8_t buffer[4];
    TMC_spi_status_t status = 0;

    if(driver.axis != axis) {
        buffer[0] = driver.axis | 0x80;
        HAL_I2C_Mem_Write(&i2c_port, tmc_addr, axis, I2C_MEMADD_SIZE_8BIT, buffer, 1, 100);

        axis = driver.axis;
    }

    buffer[0] = (reg->payload.value >> 24) & 0xFF;
    buffer[1] = (reg->payload.value >> 16) & 0xFF;
    buffer[2] = (reg->payload.value >> 8) & 0xFF;
    buffer[3] = reg->payload.value & 0xFF;

    reg->addr.write = 1;
    HAL_I2C_Mem_Write(&i2c_port, tmc_addr, (uint16_t)reg->addr.idx, I2C_MEMADD_SIZE_8BIT, buffer, 4, 100);
    reg->addr.write = 0;

    return status;
}

#endif // TRINAMIC_ENABLE && TRINAMIC_I2C

#endif // I2C_ENABLE
