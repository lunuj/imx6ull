// 头文件main.h，定义寄存器的地址
#ifndef __MAIN_H
#define __MAIN_H    

/*定义要使用的寄存器*/
// 时钟寄存器
#define CCM_CCGR0               *((volatile unsigned int*)0x020c4068)
#define CCM_BASE                (0x020C400)
#define CCM                      ((CCM_Type *)CCM_BASE)
typedef struct 
{
    volatile unsigned int CCR;
    volatile unsigned int CCDR;
    volatile unsigned int CSR;
    volatile unsigned int CCSR;
    volatile unsigned int CACRR;
    volatile unsigned int CBCDR;
    volatile unsigned int CBCMR;
    volatile unsigned int CSCMR1;
    volatile unsigned int CSCMR2;
    volatile unsigned int CSCDR1;
    volatile unsigned int CS1CDR;
    volatile unsigned int CS2CDR;
    volatile unsigned int CDCDR;
    volatile unsigned int CHSCCDR;
    volatile unsigned int CSCDR2;
    volatile unsigned int CSCDR3;    
    volatile unsigned int RESERVED_1[2];
    volatile unsigned int CDHIPR;  
    volatile unsigned int RESERVED_2[2];
    volatile unsigned int CLPCR;
    volatile unsigned int CISR;
    volatile unsigned int CIMR;
    volatile unsigned int CCOSR;
    volatile unsigned int CGPR;
    volatile unsigned int CCGR0;
    volatile unsigned int CCGR1;
    volatile unsigned int CCGR2;
    volatile unsigned int CCGR3;
    volatile unsigned int CCGR4;
    volatile unsigned int CCGR5;
    volatile unsigned int CCGR6;
    volatile unsigned int RESERVED_3[1];
    volatile unsigned int CMEOR;    
} CCM_Type;

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

