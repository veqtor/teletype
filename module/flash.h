#ifndef _FLASH_H_
#define _FLASH_H_

#include <stdint.h>

#include "globals.h"
#include "line_editor.h"
#include "teletype.h"

#define SCENE_SLOTS 32

#define SCENE_TEXT_LINES 32
#define SCENE_TEXT_CHARS LINE_EDITOR_SIZE

extern char scene_text[SCENE_TEXT_LINES][SCENE_TEXT_CHARS];

typedef const struct {
    scene_script_t script[10];
    scene_pattern_t patterns[4];
    char text[SCENE_TEXT_LINES][SCENE_TEXT_CHARS];
} nvram_scene_t;

typedef const struct {
    nvram_scene_t s[SCENE_SLOTS];
    uint8_t scene;
    tele_mode_t mode;
    uint8_t fresh;
} nvram_data_t;

extern nvram_data_t f;

uint8_t flash_is_fresh(void);
void flash_unfresh(void);
void flash_write(uint8_t preset_no);
void flash_read(uint8_t preset_no);
void flash_save_mode(tele_mode_t mode);

#endif
