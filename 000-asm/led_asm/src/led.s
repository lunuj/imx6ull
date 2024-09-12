.global _start @全局标号
_start:
@ 开启所有时钟CCM_CCGR0-6

    /*使能所有外设时钟 */
    ldr r0,=0x020c4068   @CCGR0
    ldr r1,=0xffffffff   @向CCGR0待写入的数据
    str r1,[r0]          @将r1里的数据写入r0地址的寄存器内

@设置复用IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO03 = 5
    ldr r0,=0x020e0068
    ldr r1,=0x5
    str r1,[r0]

@设置电气属性IOMUXC_SW_PAD_CTL_PAD_GPIO1_IO03 地址0x020E02F4
/*
    @  *bit 16:0 HYS关闭
    @  *bit [15:14]: 00 默认下拉
    @  *bit [13]: 0 kepper功能
    @  *bit [12]: 1 pull/keeper使能
    @  *bit [11]: 0 关闭开路输出
    @  *bit [10-8]: 000 空
    @  *bit [7:6]: 10 速度100Mhz
    @  *bit [5:3]: 110 R0/6驱动能力
    @  *bit [2:1]:  00  空
    @  *bit [0]: 0 低转换率
    @   00001 0000 1011 0000>0x10B0
 */

    ldr r0,=0x020e02f4
    ldr r1,=0x10b0
    str r1,[r0]

/*设置GPIO1_IO03为输出，
GPIO1_GDIR地址为 0x209_C004
IO03对应第2bit，值为0x8
*/
    ldr r0,=0x0209C004
    ldr r1,=0x8
    str r1,[r0]

/*设置GPIO输出高电平
GPIO1_DR,地址为0x209_C000
IO03对应第3bit，低电平时灯点亮，值为0
*/

    ldr r0,=0x0209C000
    ldr r1,=0
    str r1,[r0]

loop:
    b loop
