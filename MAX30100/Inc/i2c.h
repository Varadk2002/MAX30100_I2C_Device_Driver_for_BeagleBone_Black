/*
 * i2c.h
 *
 *  Created on: Jun 21, 2025
 *      Author: srushti
 */

#ifndef I2C_H_
#define I2C_H_

#include "stm32f4xx.h"

void I2CInit(void);
uint8_t I2CRecvDataAck(void);
uint8_t I2CRecvDataNAck(void);

void I2CInit(void);
void I2CStart(void);
void I2CRepeatStart(void);
void I2CStop(void);
void I2CSendSlaveAddr(uint8_t addr);
void I2CSendData(uint8_t data);
void I2CWrite(uint8_t addr, uint8_t data);

#endif /* I2C_H_ */
