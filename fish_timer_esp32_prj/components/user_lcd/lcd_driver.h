#ifndef _LCD_DRIVER_H_
#define _LCD_DRIVER_H

#include "stdio.h"

void lcd_init(void);
void bgd_pic_fullfill(void);
void lcd_draw_icon(int x_start, int y_start, int x_end, int y_end, const void *data);
void lcd_flush(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, void *colordata);

#endif
