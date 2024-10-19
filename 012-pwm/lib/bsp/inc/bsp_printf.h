#ifndef BSP_PRINTF_H
#define BSP_PRINTF_H

#define UART_PRINT                  UART1
#define UART_PRINT_BUF_LEN          16
void printf(char *Data, ...);

#endif // BSP_PRINTF_H