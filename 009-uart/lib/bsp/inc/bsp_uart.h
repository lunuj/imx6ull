#ifndef BSP_UART_H
#define BSP_UART_H

#include "imx6ull.h"

void Uart_init(UART_Type *uart, uint32_t baud);
void Uart_enable(UART_Type *uart);
void Uart_disable(UART_Type *uart);
void Uart_softReset(UART_Type *uart);
void Uart_putChar(UART_Type *uart, uint8_t c);
uint8_t Uart_getChar(UART_Type *uart);
void Uart_setBaudrate(UART_Type *base, unsigned int baudrate, unsigned int srcclock_hz);

#endif // BSP_UART_H

