.global _start

_start:
    /*设置处理器进入SVC模式 */
    MRS R0,CPSR @读取CPRS到R0
    BIC R0,R0,#0x1f @清除CPSR[4:0](1f为最后5位,清除哪位就把哪位置一)
    ORR R0,R0,#0x13 @或运算,10011,对应SVC模式
    MSR CPSR,R0

    /*设置SP指针 ，这里一定要注意是设置SP指针要在DDR初始化之后*/
    ldr sp,=0x80200000
    
    /*跳转到C语言main函数 */
    b main
