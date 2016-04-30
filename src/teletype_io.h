#ifndef _TELETYPE_IO_H_
#define _TELETYPE_IO_H_

#include <stdint.h>

typedef void (*update_metro_t)(int16_t, int16_t, uint8_t);
extern volatile update_metro_t update_metro;

typedef void (*update_tr_t)(uint8_t, int16_t);
extern volatile update_tr_t update_tr;

typedef void (*update_cv_t)(uint8_t, int16_t, uint8_t);
extern volatile update_cv_t update_cv;

typedef void (*update_cv_slew_t)(uint8_t, int16_t);
extern volatile update_cv_slew_t update_cv_slew;

typedef void (*update_delay_t)(uint8_t);
extern volatile update_delay_t update_delay;

typedef void (*update_s_t)(uint8_t);
extern volatile update_s_t update_s;

typedef void (*update_cv_off_t)(uint8_t, int16_t v);
extern volatile update_cv_off_t update_cv_off;

typedef void (*update_ii_t)(uint8_t, int16_t);
extern volatile update_ii_t update_ii;

typedef void (*update_scene_t)(uint8_t);
extern volatile update_scene_t update_scene;

typedef void (*update_pi_t)(void);
extern volatile update_pi_t update_pi;

typedef void (*run_script_t)(uint8_t);
extern volatile run_script_t run_script;

typedef void (*update_kill_t)(void);
extern volatile update_kill_t update_kill;

typedef void (*update_mute_t)(uint8_t, uint8_t);
extern volatile update_mute_t update_mute;

typedef void (*update_input_t)(uint8_t);
extern volatile update_input_t update_input;

#endif
