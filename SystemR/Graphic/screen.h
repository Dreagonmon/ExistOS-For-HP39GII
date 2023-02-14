/**
 * hp-39gii screen virtual framebuffer
*/
#pragma once

#include "framebuf.h"

/* MUST HAVE Defines and Functions */
#define SCR_W 256
#define SCR_H 127
/* get screen framebuffer */
gfb_FrameBuffer *get_frame_buffer();
void screen_flush();

#define INDICATE_LEFT      (1 << 0)
#define INDICATE_RIGHT     (1 << 1)
#define INDICATE_A__Z      (1 << 2)
#define INDICATE_a__z      (1 << 3) 
#define INDICATE_BUSY      (1 << 4)
#define INDICATE_TX        (1 << 5)
#define INDICATE_RX        (1 << 6)

void screen_deinit();
void screen_init_mono();
void screen_init_gray();
uint8_t check_indicator(uint8_t indicator_bit);
void set_indicator(uint8_t indicator_bit, uint8_t set_or_clear);
