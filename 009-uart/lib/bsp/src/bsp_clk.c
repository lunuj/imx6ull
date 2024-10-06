#include "bsp_clk.h"

/**
 * @brief 开启全部外设时钟
 * @retval 无
 * @note 使用位域结构体赋值，开启全部外设时钟。
 */
void Clk_enable(void)
{
    CCM->CCGR0 = 0xFFFFFFFF;
    CCM->CCGR1 = 0xFFFFFFFF;
    CCM->CCGR2 = 0xFFFFFFFF;
    CCM->CCGR3 = 0xFFFFFFFF;
    CCM->CCGR4 = 0xFFFFFFFF;
    CCM->CCGR5 = 0xFFFFFFFF;
    CCM->CCGR6 = 0xFFFFFFFF;
}

/**
 * @brief  时钟初始化
 * @retval 无
 * @note 区别于clk_enable()函数，此函数配置主频为528MHz，开启部分外设时钟
 */
void Imx6ull_clkInit(void)
{
    unsigned int reg = 0; // 过程变量，用来周转参数
    /*-------------------------主频设置-------------------------*/
    /*初始化主频为528MHz*/
    if (((CCM->CCSR >> 2) & 0x1) == 0) // 确认当前时钟使用PLL1并切换至step_clk
    {
        CCM->CCSR &= ~(1 << 8); // CCSR[bit8]清零，切换置step_clk = osc_clk = 24MHz
        CCM->CCSR |= 1 << 2;    // CCSR[2]:pll1_sw_clk_sel置1,PLL1 = step_clk
    }

    /*
    设置pll1_main_clk=1056MHz
    */
    CCM_ANALOG->PLL_ARM = ((88 << 0) & 0x7f) | (1 << 13);
    //CCM_ANALOG_PLL_ARM[DIV_SELECT](bit[6:0])=88，PLL为1056MHz 和0x7f进行与运算是比较保险。 bit[13]置1,使能时钟输出

    CCM->CACRR = 1;         // 设置CACRR[ARM_PODF]为2分频，使主频=PLL1/2 = 528MHz
    CCM->CCSR &= ~(1 << 2); // 切换至设置主时钟使PLL1_sw_clk = PLL1_main_clk=1056

    /*-----------------------主频设置完毕-----------------------*/

    /*-------------------------PFD设置-------------------------*/
    /*设置PLL2的4路PFD，PLL2主频为528,计算公式为 主频=528*18/FRAC，即FRAC = 528*18/主频
    先清零每个PFD对应的FRAC，再按下面要求设置
    PLL2_FPD3=297MHz  FRAC=32
    PLL2_PFD2=396MHz  FRAC=24
    PLL2_PFD1=594MHz  FRAC=16
    PLL2_PFD0=352MHz  FRAC=27
    */
    reg = CCM_ANALOG->PFD_528; // 获取寄存器值传递给变量reg
    reg &= 0x3F3F3F;           // 清除原有的值

    /*
    按计算结果设置参数
    */
    reg |= 32 << 24;
    reg |= 24 << 16;
    reg |= 16 << 8;
    reg |= 27 << 0;

    CCM_ANALOG->PFD_528 = reg; // 设置寄存器

    /*设置PLL3的4路PFD，PLL3主频为420,计算公式为 主频=420*18/FRAC，即FRAC = 420*18/主频
    先清零每个PFD对应的FRAC，再按下面要求设置
    PLL3_FPD3=454.7MHz  FRAC=19
    PLL3_PFD2=508.2MHz  FRAC=17
    PLL2_PFD1=540MHz  FRAC=16
    PLL2_PFD0=720MHz  FRAC=12
    */

    reg = CCM_ANALOG->PFD_480;
    reg &= 0x3F3F3F; // 将寄存器清零

    reg |= 19 << 24;
    reg |= 17 << 16;
    reg |= 16 << 8;
    reg |= 12 << 0;

    CCM_ANALOG->PFD_480 = reg;

    /*-----------------------PFD设置完毕-----------------------*/

    /*-------------------------AHB设置-------------------------*/
    /*设置AHB_CLK_ROOT=132MHz*/
    CCM->CBCMR &= ~(3 << 18); // CBCMR[pre_periph_clk_sel](bit[19:18])清零
    CCM->CBCMR |= (1 << 18);  // CBCMR[pre_periph_clk_sel](bit[19:18])设置为1,选用PLL2_PDF2=396MHz
    CCM->CBCDR &= ~(1 << 25); // CBCDR[periph_clk_sel](bit[25])设置为0,选用PLL2_PDF2。

    while (CCM->CDHIPR & (1 << 5))
        ; // 等待握手信号

#if 0
    /*这里需要关闭AHB_CLK_ROOT输出，否则会出错\
    但是没找到对应的寄存器，所以这段代如果不屏蔽掉码会出错\
    在内部bootrom中将AHB_PODF设置为3分频，即使我们不设置AHB_PODF,AHB_CLK_ROOT也是132MHz
    */
   CCM->CBCDR &= ~（7<<10);     //CBCDR[AHB_PODF](bit[12:10])清零,7=0111b,左移取反后讲指定bit位置清零
   CCM->CBCDR |= 2<<10;        //CBCDR[AHB_PODF](bit[12:10])数据为010,对应3分频
   while(CCM->CDHIPR & (1<<1));//等待握手信号
#endif

    /*-------------------------AHB设置完毕-------------------------*/

    /*--------------------------IPG设置--------------------------*/
    /*IPG_CLK_ROOT主频设置为66MHz*/
    CCM->CBCDR &= ~(3 << 8); // CBCDR[ipg_podf](9:8)清零
    CCM->CBCDR |= 1 << 8;    // CBCDR[ipg_podf](9:8)设置1,对应为2分频
    /*-------------------------IPG设置完毕------------------------*/

    /*--------------------------PERCLK设置--------------------------*/
    /*PERCLK_CLK_ROOT主频设置为66MHz*/
    CCM->CSCMR1 &= ~(1 << 6); // CSCMR1[PERIPH_CLK_SEL](bit[6])置零，选择ipg时钟源
    CCM->CSCMR1 &= ~(7 << 0); // CSCMR1[PERIPH_PODF](bit[5：0])设置为000，对应1分频
    /*-------------------------IPG设置完毕------------------------*/

    //配置UART时钟为80MHz
    CCM->CSCDR1 &= ~(1<<6);                         //此位为1使用24MHz晶振，为0使用80MHz晶振
    CCM->CSCDR1 &= ~(7<<0);
}