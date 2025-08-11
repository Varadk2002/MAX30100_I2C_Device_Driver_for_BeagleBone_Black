/*
 * i2c.c
 *
 *  Created on: Jun 21, 2025
 *      Author: srushti
 */


#include "i2c.h"

void I2CInit(void) {
	// gpio config
	// enable gpio b clock
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	// set mode as alt fn (10)
	GPIOB->MODER |= BV(2*6+1) | BV(2*7+1);
	GPIOB->MODER &= ~(BV(2*6) | BV(2*7));
	// set alt fn to AF4 (i2c)
	GPIOB->AFR[0] = (4 << (4*6)) | (4 << (4*7));
	// no pull up & pull down regr
	GPIOB->PUPDR &= ~(BV(2*6+1) | BV(2*7+1) | BV(2*6) | BV(2*7));
	// Enable open-drain for PB6 & PB7
	GPIOB->OTYPER |= BV(6) | BV(7);

	// i2c config
	// enable i2c peri clock
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
	// i2c sw reset
	I2C1->CR1 |= I2C_CR1_SWRST;
	I2C1->CR1 = 0;				// clear all CR1 bits
	// peri clock -- CR2 = 16MHz
	I2C1->CR2 |= 16 << I2C_CR2_FREQ_Pos;
	// set i2c clock -- CCR = 80 (Std mode=100KHz)
	I2C1->CCR = 80;
	I2C1->CCR &= ~I2C_CCR_FS;	// standard mode (default)
	// set Trise -- TRISE = 17
	I2C1->TRISE = 17;
	// enable ack
	I2C1->CR1 |= I2C_CR1_ACK;
	// enable i2c peri
	I2C1->CR1 |= I2C_CR1_PE;
}


void I2CStart(void) {

	I2C1->CR1 |= I2C_CR1_START;
	while(!(I2C1->SR1 & I2C_SR1_SB));
}

void I2CRepeatStart(void) {
	I2CStart();
}

void I2CStop(void) {

	I2C1->CR1 |= I2C_CR1_STOP;

	while(I2C1->SR2 & I2C_SR2_BUSY);
}

void I2CSendSlaveAddr(uint8_t addr) {

	I2C1->DR = addr;

	while(!(I2C1->SR1 & I2C_SR1_ADDR));

	(void)I2C1->SR1;
	(void)I2C1->SR2;
}

void I2CSendData(uint8_t data) {

	while(!(I2C1->SR1 & I2C_SR1_TXE));

	I2C1->DR = data;

    while (!(I2C1->SR1 & I2C_SR1_BTF));
}





uint8_t I2CRecvDataAck(void) {
	// send ack for next byte read
	I2C1->CR1 |= I2C_CR1_ACK | I2C_CR1_POS;
	// wait until data is received
	while(!(I2C1->SR1 & I2C_SR1_RXNE));
	// collect received data and return it
	return I2C1->DR;
}

uint8_t I2CRecvDataNAck(void) {
	// send no ack for next byte read
	I2C1->CR1 &= ~(I2C_CR1_ACK | I2C_CR1_POS);
	// wait until data is received
	while(!(I2C1->SR1 & I2C_SR1_RXNE));
	// collect received data and return it
	return I2C1->DR;
}

void I2CWrite(uint8_t addr, uint8_t data) {
	I2CStart();
	I2CSendSlaveAddr(addr);
	I2CSendData(data);
	I2CStop();
}
