/*
 * Uart.h
 *
 *  Created on: Aug 1, 2025
 *      Author: varad
 */

#ifndef UART_H_
#define UART_H_

#include "stm32f4xx.h"

void UartInit(uint32_t baud);
void UartPutch(int ch);
int UartGetch(void);
void UartPuts(char *str);
void UartGets(char *str);

#endif /* UART_H_ */
