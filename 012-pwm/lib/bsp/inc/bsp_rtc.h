#ifndef BSP_RTC_H
#define BSP_RTC_H

#include "imx6ull.h"

/*和时间有关的宏定义*/
#define SECONDS_IN_A_DAY        (86400)
#define SECONDS_IN_A_HOUR       (3600)
#define SECONDS_IN_A_MINUTE     (60)
#define DAYS_IN_A_YEAR          (365)
#define YEAR_RANGE_START        (1970)
#define YEAR_RANGE_END          (2099)

/*时间相关结构体*/
struct rtc_datetime
{
    unsigned short year;
    unsigned char  month;
    unsigned char  day;
    unsigned char  hour;
    unsigned char  minute;
    unsigned char  second;
};

void Rtc_init(void);                                            
void Rtc_enable(void);
void Rtc_disable(void);

uint64_t Rtc_getSeconds(void);
void Rtc_setSeconds(uint64_t seconds);

void Rtc_convertSeconds_to_Datetime(u64 seconds, struct rtc_datetime *datetime);
unsigned int Rtc_coverDate_to_Seconds(struct rtc_datetime *datetime);

unsigned char Rtc_isLeapyear(unsigned short year);

void Rtc_setDate(struct rtc_datetime *datatime);
void Rtc_getDate(struct rtc_datetime *datetime);

#endif // BSP_RTC_H