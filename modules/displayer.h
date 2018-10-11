#ifndef DISPLAYER_H
#define DISPLAYER_H

#include "system.h"
#include "pio.h"
#include "pacer.h"
#include "tinygl.h"
#include "../fonts/font5x7_1.h"
#include "string.h"
#include "navswitch.h"


/** Run display of text */
int displaytext_run ( void );


/** Display a Bitmap untill navswitch pressed */
void bitmap_display_run (uint8_t bitmap[5]);
#endif
