#include "sound.h"


tweeter_scale_t scale_table[] = TWEETER_SCALE_TABLE (TWEETER_TASK_RATE);


void tweeter_task_init (void)
{
    tweeter = tweeter_init (&tweeter_info, TWEETER_TASK_RATE, scale_table);
    pio_config_set (PIEZO_PIO, PIO_OUTPUT_LOW);
}


void tweeter_task (__unused__ void *data)
{
    pio_output_set (PIEZO_PIO, tweeter_update (tweeter));
}


void sound_task_init (void)
{
    melody = mmelody_init (&melody_info, SOUND_TASK_RATE,
               (mmelody_callback_t) tweeter_note_play, tweeter);

    mmelody_speed_set (melody, TUNE_BPM_RATE);
}


void sound_task ( __unused__ void *data )
{
    mmelody_update (melody);
}
