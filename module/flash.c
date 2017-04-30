#include "flash.h"

#include <string.h>

// asf
#include "flashc.h"

// this
#include "teletype.h"

#define FIRSTRUN_KEY 0x22

// NVRAM data structure located in the flash array.
typedef const struct {
    scene_script_t script[SCRIPT_COUNT];
    scene_pattern_t patterns[PATTERN_COUNT];
    char text[SCENE_TEXT_LINES][SCENE_TEXT_CHARS];
} nvram_scene_t;

typedef const struct {
    nvram_scene_t s[SCENE_SLOTS];
    uint8_t scene;
    uint8_t fresh;
} nvram_data_t;

static __attribute__((__section__(".flash_nvram"))) nvram_data_t f;

uint8_t flash_is_fresh(void) {
    return (f.fresh != FIRSTRUN_KEY);
}

// write fresh status
void flash_unfresh(void) {
    // blank scene to write to flash
    scene_state_t scene;
    ss_init(&scene);

    char text[SCENE_TEXT_LINES][SCENE_TEXT_CHARS];
    memset(text, 0, SCENE_TEXT_LINES * SCENE_TEXT_CHARS);

    for (uint8_t i = 0; i < SCENE_SLOTS; i++) { flash_write(i, &scene, &text); }
    preset_select = 0;
    flashc_memset8((void *)&(f.scene), preset_select, 1, true);
    flashc_memset8((void *)&(f.fresh), FIRSTRUN_KEY, 1, true);
}

void flash_write(uint8_t preset_no, scene_state_t *scene,
                 char (*text)[SCENE_TEXT_LINES][SCENE_TEXT_CHARS]) {
    flashc_memcpy((void *)&f.s[preset_no].script, ss_scripts_ptr(scene),
                  ss_scripts_size(), true);
    flashc_memcpy((void *)&f.s[preset_no].patterns, ss_patterns_ptr(scene),
                  ss_patterns_size(), true);
    flashc_memcpy((void *)&f.s[preset_no].text, text,
                  SCENE_TEXT_LINES * SCENE_TEXT_CHARS, true);
    flashc_memset8((void *)&(f.scene), preset_no, 1, true);
}

void flash_read(uint8_t preset_no, scene_state_t *scene,
                char (*text)[SCENE_TEXT_LINES][SCENE_TEXT_CHARS]) {
    memcpy(ss_scripts_ptr(scene), &f.s[preset_no].script, ss_scripts_size());
    memcpy(ss_patterns_ptr(scene), &f.s[preset_no].patterns,
           ss_patterns_size());
    memcpy(text, &f.s[preset_no].text, SCENE_TEXT_LINES * SCENE_TEXT_CHARS);
    flashc_memset8((void *)&(f.scene), preset_no, 1, true);
}

uint8_t flash_last_saved_scene() {
    return f.scene;
}

const char *flash_scene_text(uint8_t preset_no, size_t line) {
    return f.s[preset_no].text[line];
}
