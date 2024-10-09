#include "bsp_rtc.h"

void Rtc_init(void)
{
    
    SNVS->HPCOMR |= (1<<31) | (1<<8);

/*下面的部分为给RTC赋个初始值，实际过程放在按钮触发的中断里了*/
#if 1
    struct rtc_datetime Date_init;

    Date_init.year = 2024;
    Date_init.month = 10;
    Date_init.day = 9;
    Date_init.hour = 21;
    Date_init.minute = 56;
    Date_init.minute = 47;

    Rtc_setDate(&Date_init);
#endif
    Rtc_setSeconds(0);
    Rtc_enable();
}

/**
 * @brief  使能RTC
 * @retval 无
 * @note 
 */
void Rtc_enable(void)
{
    SNVS->LPCR |= (1<<0);
    while((SNVS->LPCR & 0x01) == 0);//等待使能成功
}

/**
 * @brief  关闭RTC
 * @retval 无
 * @note 
 */
void Rtc_disable(void)
{
    SNVS->LPCR &= ~(1<<0);
    while(((SNVS->LPCR) & 0x01) == 1);//等待禁用成功
}

/**
 * @brief  获取RTC存储的时间
 * @retval 距离1970.01.01的秒数
 * @note 
 */
uint64_t Rtc_getSeconds(void)
{    
    static uint64_t seconds = 0;
    seconds = (SNVS->LPSRTCMR << 17) | (SNVS->LPSRTCLR >> 15);
    return seconds;
}

/**
 * @brief  设置RTC存储的时间戳
 * @retval 无
 * @note 
 */
void Rtc_setSeconds(uint64_t seconds)
{
    SNVS->LPSRTCMR = (unsigned int)(seconds >> 17);  //高17位
    SNVS->LPSRTCLR = (unsigned int)(seconds << 15);  //低15位
}


/**
 * @brief  设置RTC时间
 * @param  datetime 要设置的时期
 * @retval 无
 * @note 
 */
void Rtc_setDate(struct rtc_datetime * datetime)
{
    unsigned int temp = SNVS->LPCR;         //当前RTC运行状态

    uint64_t seconds = 0;

    seconds =  Rtc_coverDate_to_Seconds(datetime);

    Rtc_disable();

    SNVS->LPSRTCMR = (unsigned int)(seconds >> 17);  //高17位
    SNVS->LPSRTCLR = (unsigned int)(seconds << 15);  //低15位
    
    if(temp &0x01)          //如果设置前RTC为运行，重启RTC
    {
        Rtc_enable();
    }
}

/**
 * @brief  获取RTC时间
 * @param  datetime 读取时间存放的指针
 * @retval 无
 * @note 
 */
void Rtc_getDate(struct rtc_datetime * datetime)
{
    uint64_t seconds = 0;
    seconds = Rtc_getSeconds();
    Rtc_convertSeconds_to_Datetime(seconds,datetime);
}


/**
 * @brief  判断是否是闰年
 * @param  year 年份
 * @retval 1表示是闰年 0表示不是闰年
 * @note 
 */
unsigned char Rtc_isLeapyear(unsigned short year)
{    
    unsigned char value=0;
    
    if(year % 400 == 0)
        value = 1;
    else 
    {
        if((year % 4 == 0) && (year % 100 != 0))
            value = 1;
        else 
            value = 0;
    }
    return value;
}

/**
 * @brief  转换秒数为日期
 * @param  seconds 距离1970.01.01的秒数
 * @param  datetime 转换的日期
 * @retval 无
 * @note 
 */
void Rtc_convertSeconds_to_Datetime(u64 seconds, struct rtc_datetime * datetime)
{
    u64 x;
    u64  secondsRemaining, days;
    unsigned short daysInYear;
    
    /* 每个月的天数       */
    unsigned char daysPerMonth[] = {0U, 31U, 28U, 31U, 30U, 31U, 30U, 31U, 31U, 30U, 31U, 30U, 31U};

    secondsRemaining = seconds; /* 剩余秒数初始化 */
    days = secondsRemaining / SECONDS_IN_A_DAY + 1;         /* 根据秒数计算天数,加1是当前天数 */
    secondsRemaining = secondsRemaining % SECONDS_IN_A_DAY; /*计算天数以后剩余的秒数 */

    /* 计算时、分、秒 */
    datetime->hour = secondsRemaining / SECONDS_IN_A_HOUR;
    secondsRemaining = secondsRemaining % SECONDS_IN_A_HOUR;
    datetime->minute = secondsRemaining / 60;
    datetime->second = secondsRemaining % SECONDS_IN_A_MINUTE;

    /* 计算年 */
    daysInYear = DAYS_IN_A_YEAR;
    datetime->year = YEAR_RANGE_START;
    while(days > daysInYear)
    {
        /* 根据天数计算年 */
        days -= daysInYear;
        datetime->year++;

        /* 处理闰年 */
        if (!Rtc_isLeapyear(datetime->year))
            daysInYear = DAYS_IN_A_YEAR;
        else    /*闰年，天数加一 */
            daysInYear = DAYS_IN_A_YEAR + 1;
    }
    /*根据剩余的天数计算月份 */
    if(Rtc_isLeapyear(datetime->year)) /* 如果是闰年的话2月加一天 */
        daysPerMonth[2] = 29;

    for(x = 1; x <= 12; x++)
    {
        if (days <= daysPerMonth[x])
        {
            datetime->month = x;
            break;
        }
        else
        {
            days -= daysPerMonth[x];
        }
    }

    datetime->day = days;

}

/**
 * @brief  日期转换为时间戳
 * @param  datetime 需要转换的日期
 * @retval 返回时间戳
 * @note 
 */
unsigned int Rtc_coverDate_to_Seconds(struct rtc_datetime * datetime)
{    
    unsigned short i = 0;
    unsigned int seconds = 0;
    unsigned int days = 0;
    unsigned short monthdays[] = {0U, 0U, 31U, 59U, 90U, 120U, 151U, 181U, 212U, 243U, 273U, 304U, 334U};
    
    for(i = 1970; i < datetime->year; i++)
    {
        days += DAYS_IN_A_YEAR;         /* 平年，每年365天 */
        if(Rtc_isLeapyear(i)) days += 1;/* 闰年多加一天         */
    }

    days += monthdays[datetime->month];
    if(Rtc_isLeapyear(i) && (datetime->month >= 3)) days += 1;/* 闰年，并且当前月份大于等于3月的话加一天 */

    days += datetime->day - 1;

    seconds = days * SECONDS_IN_A_DAY + 
                datetime->hour * SECONDS_IN_A_HOUR +
                datetime->minute * SECONDS_IN_A_MINUTE +
                datetime->second;

    return seconds;    
}