#include "lcd.h"
#include "lcd_app.h"

// Temp
#define pos_Temp_int_x 84
#define pos_Temp_int_y 47
#define pos_Temp_float_x 121
#define pos_Temp_float_y 47
#define RES_H_Temp 15
#define RES_V_Temp 21

// Water
#define pos_Water_int_x 231
#define pos_Water_int_y 47
#define pos_Water_float_x 269
#define pos_Water_float_y 47
#define RES_H_Water 15
#define RES_V_Water 21

// Time
#define pos_year_x 18
#define pos_year_y 12
#define pos_month_x 72
#define pos_month_y 12
#define pos_day_x 111
#define pos_day_y 12
#define pos_week_x 188
#define pos_week_y 10
#define pos_hour_x 210
#define pos_hour_y 12
#define pos_min_x 238
#define pos_min_y 12
#define pos_sec_x 264
#define pos_sec_y 12
#define pos_AM_PM_x 286
#define pos_AM_PM_y 15
#define RES_H_Time 8
#define RES_V_Time 11
#define RES_H_Week 15
#define RES_V_Week 15
#define RES_H_AM_PM 15
#define RES_V_AM_PM 8

// Note
#define pos_note_x 125
#define pos_note_y 108
#define RES_H_NOTE 36
#define RES_V_NOTE 38

extern const uint8_t temp_num_map[];
extern const uint8_t water_num_map[];
extern const uint8_t time_num_map[];
extern const uint8_t week_num_map[];
extern const uint8_t AM_PM_map[];
extern const uint8_t note_map[];

// orange
void display_temp(float tempnum)
{
    int num_set = (int)(tempnum * 100);
    int xstart = pos_Temp_int_x, ystart = pos_Temp_int_y;

    lcd_draw_icon(xstart,
                  ystart,
                  xstart + RES_H_Temp,
                  ystart + RES_V_Temp,
                  temp_num_map + RES_H_Temp * RES_V_Temp * (num_set / 1000 % 10) * 2);
    xstart += RES_H_Temp;
    lcd_draw_icon(xstart,
                  ystart,
                  xstart + RES_H_Temp,
                  ystart + RES_V_Temp,
                  temp_num_map + RES_H_Temp * RES_V_Temp * (num_set / 100 % 10) * 2);

    xstart = pos_Temp_float_x;
    lcd_draw_icon(xstart,
                  ystart,
                  xstart + RES_H_Temp,
                  ystart + RES_V_Temp,
                  temp_num_map + RES_H_Temp * RES_V_Temp * (num_set / 10 % 10) * 2);
    xstart += RES_H_Temp;
    lcd_draw_icon(xstart,
                  ystart,
                  xstart + RES_H_Temp,
                  ystart + RES_V_Temp,
                  temp_num_map + RES_H_Temp * RES_V_Temp * (num_set % 10) * 2);
}

// blue
void display_water(float waternum)
{
    int num_set = (int)(waternum * 100);
    int xstart = pos_Water_int_x, ystart = pos_Water_int_y;

    lcd_draw_icon(xstart,
                  ystart,
                  xstart + RES_H_Water,
                  ystart + RES_V_Water,
                  water_num_map + RES_H_Water * RES_V_Water * (num_set / 1000 % 10) * 2);
    xstart += RES_H_Water;
    lcd_draw_icon(xstart,
                  ystart,
                  xstart + RES_H_Water,
                  ystart + RES_V_Water,
                  water_num_map + RES_H_Water * RES_V_Water * (num_set / 100 % 10) * 2);

    xstart = pos_Water_float_x;
    lcd_draw_icon(xstart,
                  ystart,
                  xstart + RES_H_Water,
                  ystart + RES_V_Water,
                  water_num_map + RES_H_Water * RES_V_Water * (num_set / 10 % 10) * 2);
    xstart += RES_H_Water;
    lcd_draw_icon(xstart,
                  ystart,
                  xstart + RES_H_Water,
                  ystart + RES_V_Water,
                  water_num_map + RES_H_Water * RES_V_Water * (num_set % 10) * 2);
}

// grey
void display_time(void)
{
    int year = 2023, month = 12, day = 25;
    int week = 6;
    int hour = 8, min = 30, sec = 0;
    int xstart = pos_year_x, ystart = pos_year_y;
    int AM_PM = 0;

    // year
    lcd_draw_icon(xstart,
                  ystart,
                  xstart + RES_H_Time,
                  ystart + RES_V_Time,
                  time_num_map + RES_H_Time * RES_V_Time * (year / 1000 % 10) * 2);
    xstart += RES_H_Time;
    lcd_draw_icon(xstart,
                  ystart,
                  xstart + RES_H_Time,
                  ystart + RES_V_Time,
                  time_num_map + RES_H_Time * RES_V_Time * (year / 100 % 10) * 2);
    xstart += RES_H_Time;
    lcd_draw_icon(xstart,
                  ystart,
                  xstart + RES_H_Time,
                  ystart + RES_V_Time,
                  time_num_map + RES_H_Time * RES_V_Time * (year / 10 % 10) * 2);
    xstart += RES_H_Time;
    lcd_draw_icon(xstart,
                  ystart,
                  xstart + RES_H_Time,
                  ystart + RES_V_Time,
                  time_num_map + RES_H_Time * RES_V_Time * (year % 10) * 2);
    // month
    xstart = pos_month_x;
    lcd_draw_icon(xstart,
                  ystart,
                  xstart + RES_H_Time,
                  ystart + RES_V_Time,
                  time_num_map + RES_H_Time * RES_V_Time * (month / 10 % 10) * 2);
    xstart += RES_H_Time;
    lcd_draw_icon(xstart,
                  ystart,
                  xstart + RES_H_Time,
                  ystart + RES_V_Time,
                  time_num_map + RES_H_Time * RES_V_Time * (month % 10) * 2);
    // day
    xstart = pos_day_x;
    lcd_draw_icon(xstart,
                  ystart,
                  xstart + RES_H_Time,
                  ystart + RES_V_Time,
                  time_num_map + RES_H_Time * RES_V_Time * (day / 10 % 10) * 2);
    xstart += RES_H_Time;
    lcd_draw_icon(xstart,
                  ystart,
                  xstart + RES_H_Time,
                  ystart + RES_V_Time,
                  time_num_map + RES_H_Time * RES_V_Time * (day % 10) * 2);
    // hour
    xstart = pos_hour_x;
    lcd_draw_icon(xstart,
                  ystart,
                  xstart + RES_H_Time,
                  ystart + RES_V_Time,
                  time_num_map + RES_H_Time * RES_V_Time * (hour / 10 % 10) * 2);
    xstart += RES_H_Time;
    lcd_draw_icon(xstart,
                  ystart,
                  xstart + RES_H_Time,
                  ystart + RES_V_Time,
                  time_num_map + RES_H_Time * RES_V_Time * (hour % 10) * 2);
    // min
    xstart = pos_min_x;
    lcd_draw_icon(xstart,
                  ystart,
                  xstart + RES_H_Time,
                  ystart + RES_V_Time,
                  time_num_map + RES_H_Time * RES_V_Time * (min / 10 % 10) * 2);
    xstart += RES_H_Time;
    lcd_draw_icon(xstart,
                  ystart,
                  xstart + RES_H_Time,
                  ystart + RES_V_Time,
                  time_num_map + RES_H_Time * RES_V_Time * (min % 10) * 2);
    // sec
    xstart = pos_sec_x;
    lcd_draw_icon(xstart,
                  ystart,
                  xstart + RES_H_Time,
                  ystart + RES_V_Time,
                  time_num_map + RES_H_Time * RES_V_Time * (sec / 10 % 10) * 2);
    xstart += RES_H_Time;
    lcd_draw_icon(xstart,
                  ystart,
                  xstart + RES_H_Time,
                  ystart + RES_V_Time,
                  time_num_map + RES_H_Time * RES_V_Time * (sec % 10) * 2);
    // AM_PM
    xstart = pos_AM_PM_x;
    ystart = pos_AM_PM_y;
    lcd_draw_icon(xstart,
                  ystart,
                  xstart + RES_H_AM_PM,
                  ystart + RES_V_AM_PM,
                  AM_PM_map + RES_H_AM_PM * RES_V_AM_PM * (AM_PM) * 2);

    // week
    xstart = pos_week_x;
    ystart = pos_week_y;
    lcd_draw_icon(xstart,
                  ystart,
                  xstart + RES_H_Week,
                  ystart + RES_V_Week,
                  week_num_map + RES_H_Week * RES_V_Week * (week - 1) * 2);
}

// note
void display_note(void)
{
     int xstart = pos_note_x, ystart = pos_note_y;

    lcd_draw_icon(xstart,
                  ystart,
                  xstart + RES_H_NOTE,
                  ystart + RES_V_NOTE,
                  note_map + RES_H_NOTE * RES_V_NOTE * (8) * 2);
    xstart += RES_H_NOTE;
    lcd_draw_icon(xstart,
                  ystart,
                  xstart + RES_H_NOTE,
                  ystart + RES_V_NOTE,
                  note_map + RES_H_NOTE * RES_V_NOTE * (8) * 2);
}

