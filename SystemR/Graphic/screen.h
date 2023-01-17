/**
 * hp-39gii screen virtual framebuffer
*/
#pragma once

#include "framebuf.h"

extern gfb_GrayFrameBuffer *screen_frame;

void screen_deinit();
void screen_init_mono();
void screen_init_gray();
void screen_flush();
