/**
 * hp-39gii screen virtual framebuffer
*/
#pragma once

#include "framebuf.h"

extern GrayFrameBuffer *screen_frame;

void screen_init();
void screen_flush();
