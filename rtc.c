#include "rtc.h"
int hook_rtc=3;

int rtc_subscribe_int(uint8_t *bit_no){
    *bit_no = hook_rtc;
    if(sys_irqsetpolicy(RTC_IRQ , IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_rtc) != OK)
        return 1;
    return 0;
}

int rtc_unsubscribe_int(){
    if(sys_irqdisable(&hook_rtc) != OK)
        return 1;
    if(sys_irqrmpolicy(&hook_rtc)!=OK)
        return 1;
    return 0;
}

int read_second(){
    uint32_t second;
    if(sys_outb(RTC_ADDR_REG, SECOND_REG) != OK)
        return 1;
    else if(sys_inb(RTC_DATA_REG, &second) != OK)
        return 1;
    else
        return second;
}

int read_minute(){
    uint32_t minute;
    if(sys_outb(RTC_ADDR_REG, MINUTE_REG) != OK)
        return 1;
    else if(sys_inb(RTC_DATA_REG, &minute) != OK)
        return 1;
    else
        return minute;
}

int read_hour(){
    uint32_t hour;
    if(sys_outb(RTC_ADDR_REG, HOUR_REG) != OK)
        return 1;
    else if(sys_inb(RTC_DATA_REG, &hour) != OK)
        return 1;
    else
        return hour;
}

int read_year(){
    uint32_t year;

    if(sys_outb(RTC_ADDR_REG, YEAR_REG) != OK)
        return 1;
    else if(sys_inb(RTC_DATA_REG, &year) != OK)
        return 1;
    else
        return year;
}

int read_month(){
    uint32_t month;
    if(sys_outb(RTC_ADDR_REG, MONTH_REG) != OK)
        return 1;
    else if(sys_inb(RTC_DATA_REG, &month) != OK)
        return 1;
    else
        return month;
}

int read_day(){
    uint32_t day;
    if(sys_outb(RTC_ADDR_REG, DAY_REG) != OK)
        return 1;
    else if(sys_inb(RTC_DATA_REG, &day) != OK)
        return 1;
    else
        return day;
}

bool is_updating(){
    uint32_t a_reg;
    if(sys_outb(RTC_ADDR_REG, REG_A) != OK)
        return 1;
    else if(sys_inb(RTC_DATA_REG, &a_reg) != OK)
        return 1;
    return (a_reg & UIP_MASK);
}

uint32_t convert_to_decimal(uint32_t date){
    return (date - 6 * (date>> 4));
}

Date read_date(){
    if(is_updating())
        usleep(TIME_TO_UPDATE);
    Date date;
    uint32_t b_reg;
    if(sys_outb(RTC_ADDR_REG, REG_B) != OK)
        return date;
    else if(sys_inb(RTC_DATA_REG, &b_reg) != OK)
        return date;
    if ((b_reg & BCD_MASK) == 0)
    {
        date.hour=convert_to_decimal(read_hour());
        date.minute=convert_to_decimal(read_minute());
        date.second=convert_to_decimal(read_second());
        date.year=convert_to_decimal(read_year());
        date.month=convert_to_decimal(read_month());
        date.day=convert_to_decimal(read_day());
    }
    else
    {
        date.hour=read_hour();
        date.minute=read_minute();
        date.second=read_second();
        date.year=read_year();
        date.month=read_month();
        date.day=read_day();
    }
    return date;

}
