#ifndef _TELETYPE_IO_H_
#define _TELETYPE_IO_H_

#include <stdbool.h>
#include <stdint.h>

// These functions are for interacting with the teletype hardware, each target
// must provide it's own implementation

extern void tele_metro(int16_t, int16_t, uint8_t);
extern void tele_tr(uint8_t i, int16_t v);
extern void tele_cv(uint8_t i, int16_t v, uint8_t s);
extern void tele_cv_slew(uint8_t i, int16_t v);
extern void tele_delay(uint8_t i);
extern void tele_s(uint8_t i);
extern void tele_cv_off(uint8_t i, int16_t v);
extern void tele_ii(uint8_t i, int16_t d);
extern void tele_ii_tx(uint8_t addr, uint8_t *data, uint8_t l);
extern void tele_ii_tx_now(uint8_t addr, uint8_t *data, uint8_t l);
extern void tele_ii_rx(uint8_t addr, uint8_t *data, uint8_t l);
extern void tele_scene(uint8_t i);
extern void tele_pi(void);
extern void tele_kill(void);
extern void tele_mute(void);
extern bool tele_get_input_state(uint8_t);

#endif
