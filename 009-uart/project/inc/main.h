// 头文件main.h，定义寄存器的地址
#ifndef __MAIN_H
#define __MAIN_H    

#include "bsp_led.h"
#include "bsp_delay.h"
#include "bsp_clk.h"
#include "bsp_beep.h"
#include "bsp_key.h"
#include "bsp_int.h"
#include "bsp_exti.h"
#include "bsp_epit.h"
#include "bsp_keyfilter.h"
#include "bsp_gpt.h"
#include "bsp_uart.h"

void raise(int sig_nr);

#endif

