#ifndef _KEYBOARD_HELPER_
#define _KEYBOARD_HELPER_

#include <stdbool.h>
#include <stdint.h>

// asf
#include "conf_usb_host.h"  // needed in order to include "usb_protocol_hid.h"
#include "usb_protocol_hid.h"

// small functions defined static inline, so that each compilation unit gets
// it's own copy and may inline (no LTO in gcc 4.47)

static inline bool match_no_mod(uint8_t mod, uint8_t key,
                                uint8_t required_key) {
    return (mod == HID_MODIFIER_NONE) && (key == required_key);
}

static inline bool match_shift(uint8_t mod, uint8_t key, uint8_t required_key) {
    return (mod == HID_MODIFIER_LEFT_SHIFT ||
            mod == HID_MODIFIER_RIGHT_SHIFT) &&
           (key == required_key);
}

static inline bool match_alt(uint8_t mod, uint8_t key, uint8_t required_key) {
    return (mod == HID_MODIFIER_LEFT_ALT || mod == HID_MODIFIER_RIGHT_ALT) &&
           (key == required_key);
}
#endif
