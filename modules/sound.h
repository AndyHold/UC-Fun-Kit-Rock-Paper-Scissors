#ifndef SOUND_H
#define SOUND_H

#include "system.h"
#include "tweeter.h"
#include "pio.h"
#include "mmelody.h"


#define PIEZO_PIO PIO_DEFINE (PORT_D, 6)


enum {TWEETER_TASK_RATE = 20000};
enum {SOUND_TASK_RATE = 100};
enum {TUNE_BPM_RATE = 120};


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
