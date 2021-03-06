#ifndef INITIALIZER_H
#define INITIALIZER_H


#include "system.h"
#include "pio.h"
#include "pacer.h"
#include "tinygl.h"
#include "../fonts/font5x7_1.h"
#include "string.h"
#include "navswitch.h"
#include "ir_uart.h"
#include "button.h"
#include "led.h"


#define BTN_PIO PIO_DEFINE (PORT_D, 7)


/** Initialize everything */
void initialize_all (uint16_t pacer_rate, uint16_t message_rate);


/** Initialize common display elements for text */
void text_display_init ( uint16_t pacer_rate, uint16_t message_rate );


/** Initialize Bitmap Display */
void bitmap_display_init ( void );


#endif
