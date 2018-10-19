/** @file   sound.c
    @author A.Holden & R.Yoshinari
    @date   17 Oct 2018
    @brief  Sound Module
    @note   This is the sound module containing methods to
    play sound with a piezo buzzer using pins 6 and ground of the fun
    kit.
*/


/** Include sound.h file which contains all other imported modules. */
#include "sound.h"


/** Initialize the tweeter scale table used by the mmelody module */
tweeter_scale_t scale_table[] = TWEETER_SCALE_TABLE (TWEETER_TASK_RATE);


/** Initialize tweeter task. */
void tweeter_task_init (void)
{
    tweeter = tweeter_init (&tweeter_info, TWEETER_TASK_RATE, scale_table);
    pio_config_set (PIEZO_PIO, PIO_OUTPUT_LOW);
}


/** Tweeter task to be performed. */
void tweeter_task (__unused__ void *data)
{
    pio_output_set (PIEZO_PIO, tweeter_update (tweeter));
}


/** Initialize sound task. */
void sound_task_init (void)
{
    melody = mmelody_init (&melody_info, SOUND_TASK_RATE,
               (mmelody_callback_t) tweeter_note_play, tweeter);

    mmelody_speed_set (melody, TUNE_BPM_RATE);
}


/** Sound task to be performed. */
void sound_task ( __unused__ void *data )
{
    mmelody_update (melody);
}
