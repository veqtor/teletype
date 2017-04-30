#ifndef _FLASH_H_
#define _FLASH_H_

#include <stdint.h>

#include "globals.h"
#include "line_editor.h"
#include "teletype.h"

#define SCENE_SLOTS 32

uint8_t flash_is_fresh(void);
void flash_unfresh(void);
void flash_read(uint8_t preset_no, scene_state_t *scene,
                char (*text)[SCENE_TEXT_LINES][SCENE_TEXT_CHARS]);
void flash_write(uint8_t preset_no, scene_state_t *scene,
                 char (*text)[SCENE_TEXT_LINES][SCENE_TEXT_CHARS]);
uint8_t flash_last_saved_scene(void);
const char *flash_scene_text(uint8_t preset_no, size_t line);

#endif
