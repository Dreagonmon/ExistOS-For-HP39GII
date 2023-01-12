#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "sys_llapi.h"

#define max(a,b) ((a) >= (b) ? (a) : (b))
#define min(a,b) ((a) <= (b) ? (a) : (b))
#define abs(x) (((int32_t)x) < 0 ? (-x) : (x))

static uint8_t spixel[] = { 0 };
static uint8_t *svram = NULL; 
void api_vram_initialize(uint8_t *vram_addr)
{
    svram = vram_addr;
}

void api_vram_flush(void)
{
    if(svram)
    {
        // ll_disp_put_area(svram, 0, 0, 255, 126);
    }
}

void *api_vram_get_current(void)
{
    return svram;
}

void api_vram_clear(uint8_t color)
{
    // memset(svram, color, 256 * 127);
    // api_vram_flush();
	uint32_t x, y;
	for (y = 0; y < 127; y++) {
		for (x = 0; x < 256; x++) {
			spixel[0] = color;
			ll_disp_put_area(spixel, x, y, x, y);
		}
	}
}

static inline void setPixelUnsafe(uint32_t x, uint32_t y, uint8_t c)
{
    // svram[(x) + 256 * (y)] = c;
	spixel[0] = c;
	ll_disp_put_area(spixel, x, y, x, y);
}

void api_vram_set_pixel(uint32_t x, uint32_t y, uint8_t c)
{
    // if((x < 256) && (y < 127))
    //    svram[(x) + 256 * (y)] = c;
	if((x < 256) && (y < 127)) {
		spixel[0] = c;
		ll_disp_put_area(spixel, x, y, x, y);
	}
}



void api_vram_draw_HLine(int32_t y, int32_t x1, int32_t x2, uint8_t c)
{
	unsigned int _x1, _x2;
	if((x1 & x2) >> 31 || x1 + x2 >= 256 || (unsigned)y > 127)
	{
		return;
	}
	
	if(x1 < x2)
	{
		_x1 = max(x1, 0);
		_x2 = min(x2, 255);
	}
	else
	{
		_x1 = max(x2, 0);
		_x2 = min(x1, 255);
	}
	for(; _x1 <= _x2; _x1++)
		setPixelUnsafe(_x1, y, c);
}

void api_vram_draw_VLine(int32_t x, int32_t y1, int32_t y2, uint8_t c)
{
	unsigned int _y1, _y2;
	if((y1 & y2) >> 31 || y1 + y2 >= 128 || (unsigned)x > 255)
	{
		return;
	}
	
	if(y1 < y2)
	{
		_y1 = max(y1, 0);
		_y2 = min(y2, 127);
	}
	else
	{
		_y1 = max(y2, 0);
		_y2 = min(y1, 127);
	}
	for(; _y1 <= _y2; _y1++)
		setPixelUnsafe(x, _y1, c);
}

void api_vram_draw_line(int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint8_t c)
{
	int dx = abs(x2-x1);
	int dy = abs(y2-y1);
	int sx = (x1 < x2)?1:-1;
	int sy = (y1 < y2)?1:-1;
	int err = dx-dy;
	int e2;

	while (!(x1 == x2 && y1 == y2))
	{
		api_vram_set_pixel(x1,y1,c);
		e2 = 2*err;
		if (e2 > -dy)
		{		 
			err = err - dy;
			x1 = x1 + sx;
		}
		if (e2 < dx)
		{		 
			err = err + dx;
			y1 = y1 + sy;
		}
	}
}

void api_vram_fill_rect(int32_t x, int32_t y, int32_t w, int32_t h, uint8_t c)
{
	unsigned int _x = max(x, 0), _y = max(y, 0), _w = min(256 - _x, w - _x + x), _h = min(127 - _y, h - _y + y), i, j;
	if(_x < 256 && _y < 127)
	{
		for(j = _y; j < _y + _h; j++)
			for(i = _x; i < _x + _w; i++)
				setPixelUnsafe(i, j, c);
	}
}


int api_get_key(int check_key)
{
    uint32_t keys = ll_vm_check_key();
    uint32_t keys2 = keys;
    uint16_t key, press;
    key = keys & 0xFFFF;
    press = keys >> 16;

    if(check_key >= 0)
    {
        if(check_key == key)
        {
            return press;
        }
    }else{
        while((keys = ll_vm_check_key()) == keys2){
            ;
        }
        return keys & 0xFFFF;
    }
    return 0;
}
