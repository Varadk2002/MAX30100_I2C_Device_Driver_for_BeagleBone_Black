/*
 * max30100.h
 *
 *  Created on: 26-Jun-2025
 *      Author: srushti
 */

#ifndef MAX30100_H_
#define MAX30100_H_


#include "i2c.h"
#include <stdio.h>
#include<stm32f4xx.h>

void max30100_write(uint8_t reg,uint8_t val);
uint8_t max30100_read(uint8_t reg);
void max30100_init(void);
void max30100_read_fifo(void);
float calculate_spo2(void);
void max30100_read_and_calculate_spo2(void);

#define MAX30100_Write_ADDR 0xAE
#define MAX30100_Read_ADDR 0xAF
#define mode_config 0x06
#define spo2_only 0x03
#define spo2_config 0x07
#define led_config 0x09
#define int_status 0x00
#define MAX30100_FIFO_DATA 0x05
#define NUM_SAMPLES 16
#define NO_FINGER_THRESHOLD 300

#endif /* MAX30100_H_ */
