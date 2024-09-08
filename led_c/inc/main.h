// 头文件main.h，定义寄存器的地址
#ifndef __MAIN_H
#define __MAIN_H    

/*定义要使用的寄存器*/
// 时钟寄存器
#define CCM_CCGR0               *((volatile unsigned int*)0x020c4068)

// GPIO复用、电气属性寄存器
#define SW_MUX_GPIO1_IO03       *((volatile unsigned int *)0x020E0068)
#define SW_PAD_GPIO1_IO03       *((volatile unsigned int *)0x020E02F4)

// GPIO1属性寄存器
#define GPIO1_DR                *((volatile unsigned int *)0X0209C000)
#define GPIO1_GDIR              *((volatile unsigned int *)0X0209C004)
#define GPIO1_PSR               *((volatile unsigned int *)0X0209C008)
#define GPIO1_ICR1              *((volatile unsigned int *)0X0209C00C)
#define GPIO1_ICR2              *((volatile unsigned int *)0X0209C010)
#define GPIO1_IMR               *((volatile unsigned int *)0X0209C014)
#define GPIO1_ISR               *((volatile unsigned int *)0X0209C018)
#define GPIO1_EDGE_SEL          *((volatile unsigned int *)0X0209C01C)

#endif

