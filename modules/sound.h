/** @file   sound.h
    @author A.Holden & R.Yoshinari
    @date   17 Oct 2018
    @brief  Sound Module
    @note   This is the sound module containing methods to
    play sound with a piezo buzzer using pins 6 and ground of the fun
    kit.
*/


#ifndef SOUND_H
#define SOUND_H


/** All included modules required by sound.c */
#include "system.h"
#include "tweeter.h"
#include "pio.h"
#include "mmelody.h"


/** Define piezo on pin 6 of port D and use ground for the other pin */
#define PIEZO_PIO PIO_DEFINE (PORT_D, 6)


/** Initialize the task rates for sound */
enum {TWEETER_TASK_RATE = 20000};
enum {SOUND_TASK_RATE = 100};
enum {TUNE_BPM_RATE = 250};


/** Initialize tweeter task. */
void tweeter_task_init ( void );


/** Initialize sound task. */
void sound_task_init (void);


/** Initialize sound objects. */
tweeter_t tweeter;
mmelody_t melody;
mmelody_obj_t melody_info;
tweeter_obj_t tweeter_info;


/** Initialize sound objects. */
tweeter_t tweeter;
mmelody_t melody;
mmelody_obj_t melody_info;
tweeter_obj_t tweeter_info;


/** Tweeter task to be performed. */
void tweeter_task (__unused__ void *data);


/** Sound task to be performed. */
void sound_task ( __unused__ void *data );


#endif
