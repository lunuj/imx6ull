#include "bsp_delay.h"
#include "bsp_gpt.h"

void Delay_init(GPT_Type * gpt)
{
    Gpt_init(gpt, 65);
    Gpt_setCmp0(gpt, 0xffffffff);
    Gpt_enableIrq(gpt, GPT_IRQ_OUT1);
    Gpt_startCounter(gpt);
}

void Delay_us(GPT_Type * gpt, uint32_t us)
{
    unsigned long oldcnt,newcnt;
    unsigned long tcntvalue = 0;

    oldcnt = gpt->CNT;                                //函数启动是GPT1计数值 

    while(1)
    {
        newcnt = gpt->CNT;                            //当前计数值             
        if(newcnt != oldcnt)
        {
            if(newcnt>oldcnt)                          //未溢出
            {
                tcntvalue += newcnt-oldcnt;            //newcnt-oldcnt=1,tcntvalue加1           
            }
            else                                       //溢出
            {
                tcntvalue += 0xFFFFFFFF -oldcnt + newcnt +1;
            }

            oldcnt = newcnt;                           
            if(tcntvalue >= us)                       //延时值超过指定微秒值
            {
               break; 
            }
        } 
    }
}

void Delay_ms(GPT_Type * gpt, uint32_t ms)
{
    Delay_init(gpt);
    Gpt_startCounter(gpt);
    for (uint32_t i = 0; i < ms; i++)
    {
        while((Gpt_getInterruptStatus(gpt) & GPT_IRQ_OUT1) == GPT_IRQ_OUT1);
        Gpt_clearInterruptStatus(gpt, GPT_IRQ_ALL);
    }
    Gpt_stopCounter(gpt);
}

void Delay_s(GPT_Type * gpt, uint32_t s)
{
    Delay_ms(gpt, 1000*s);
}