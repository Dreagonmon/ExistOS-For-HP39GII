#include <stdio.h>
#include <stdint.h>

#define LCD_PIX_W   256
#define LCD_PIX_H   127

#ifdef __cplusplus
extern "C" {
#endif

void api_vram_initialize(uint8_t *vram_addr);
void *api_vram_get_current(void);
void api_vram_flush(void);
void api_vram_clear(uint8_t color);
void api_vram_set_pixel(uint32_t x, uint32_t y, uint8_t c);
void api_vram_draw_HLine(int32_t y, int32_t x1, int32_t x2, uint8_t c);
void api_vram_draw_VLine(int32_t x, int32_t y1, int32_t y2, uint8_t c);
void api_vram_draw_line(int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint8_t c);
void api_vram_fill_rect(int32_t x, int32_t y, int32_t w, int32_t h, uint8_t c);



int api_get_key(int check_key);


#ifdef __cplusplus
}
#endif

