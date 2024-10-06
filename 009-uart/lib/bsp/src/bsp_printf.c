#include <stdarg.h>
#include "bsp_printf.h"
#include "bsp_uart.h"

/**
 * @brief  printf格式化输出
 * @param  Data 输出字符串
 * @retval 无
 * @note 注意，此函数目前仅支持\\r、\\n、%c、%s、%x，且数据格式仅支持32位
 */
void printf(char * Data, ...)
{
    uint32_t d;
    uint8_t buf[UART_PRINT_BUF_LEN];
    uint8_t txdata;
    va_list ap;
    va_start(ap, Data);
    while (*Data != 0)
    {
        if (*Data == 0x5c)
        {
            switch (*++Data)
            {
            case 'r':
                txdata = 0x0d;
                Uart_putChar(UART_PRINT, txdata);
                Data++;
                break;
            case 'n':
                txdata = 0x0a;
                Uart_putChar(UART_PRINT, txdata);
                Data++;
                break;
            default:
                Data++;
                break;
            }
        }
        else if (*Data == '%')
        {
            switch (*++Data)
            {
            case 'd': // 优化：只保留 %d 分支
                for (uint8_t i = 0; i < UART_PRINT_BUF_LEN; i++)
                {
                    buf[i] = '\0';
                }
                
                d = va_arg(ap, uint32_t);
                if (d == 0)
                {
                    buf[0] = 0;
                } else{
                    if (d < 0)
                    {
                        Uart_putChar(UART_PRINT, '-');
                        d = (-1)*d;
                    }
                    uint8_t int_len = 0;
                    uint32_t themp = d;
                    while (themp > 0)
                    {
                        int_len++;
                        themp = themp/10;
                    }
                    for (uint16_t i = 0; i < int_len; i++)
                    {
                        buf[UART_PRINT_BUF_LEN - 1 - i] = d%10 + '0';
                        d = d/10;
                    }
                    for (uint16_t i = 0; i < UART_PRINT_BUF_LEN; i++)
                    {
                        if (buf[i] != '\0')
                        {
                            Uart_putChar(UART_PRINT, buf[i]);
                        }
                    }
                    Data++;
                    break;
                }
            case 'c': // 优化：只保留 %c 分支
                {
                    Uart_putChar(UART_PRINT, va_arg(ap, int));
                    Data++;
                    break;
                }
            case 's': // 优化：只保留 %s 分支
                {
                    char * s = va_arg(ap, void *);
                    while (*s != '\0')
                    {
                        Uart_putChar(UART_PRINT, *s);
                        s++;
                    }
                    Data++;
                    break;
                }
            case 'x': // 优化：只保留 %x 分支
                {
                    for (uint8_t i = 0; i < UART_PRINT_BUF_LEN; i++)
                    {
                        buf[i] = '\0';
                    }
                    d = va_arg(ap, uint32_t);

                    Uart_putChar(UART_PRINT, '0');
                    Uart_putChar(UART_PRINT, 'X');

                    uint8_t int_len = 0;
                    uint32_t themp = d;
                    while (themp > 0)
                    {
                        int_len++;
                        themp = themp/16;
                    }
                    if (int_len%2!=0)
                    {
                        Uart_putChar(UART_PRINT, '0');
                    }
                    
                    for (uint16_t i = 0; i < int_len; i++)
                    {
                        buf[UART_PRINT_BUF_LEN - 1 - i] = (d%16 > 9) ? (d%16  - 10 + 'A') : (d%16 + '0');
                        d = d/16;
                    }
                    for (uint16_t i = 0; i < UART_PRINT_BUF_LEN; i++)
                    {
                        if (buf[i] != '\0')
                        {
                            Uart_putChar(UART_PRINT, buf[i]);
                        }
                    }
                    Data++;
                    break;
                }
            default:
                Data++;
                break;
            }
        }
        else
        {
            Uart_putChar(UART_PRINT, *((unsigned char *)Data));
            Data++;
        }
    }
}
