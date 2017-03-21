#ifndef _HELP_MODE_H_
#define _HELP_MODE_H_

#include <stdbool.h>
#include <stdint.h>

void process_help_keys(uint8_t key, uint8_t mod_key, bool is_held_key);
void screen_refresh_help(void);

#endif
