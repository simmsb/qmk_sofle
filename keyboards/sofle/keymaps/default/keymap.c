#include QMK_KEYBOARD_H

#include "print.h"
#include "keymap_combo.h"
#include "transactions.h"
#include "raw_hid.h"
#include "usb_descriptor.h"
#include <string.h>

enum sofle_layers {
    BASE,
    SYMB,
    NUMB,
};

#define KC_SYMB_SPC LT(SYMB, KC_SPC)
#define KC_NUMB_SPC LT(NUMB, KC_SPC)

#define KC_LCTL_F6 MT(MOD_LCTL, KC_F6)
#define KC_C_DOWN C(KC_DOWN)
#define KC_C_UP C(KC_UP)

enum custom_keycodes {
    KC_PRVWD = SAFE_RANGE,
    KC_NXTWD,
};

#define SCREEN_BUFFER_LENGTH 512

uint8_t screen_buf[SCREEN_BUFFER_LENGTH] = {0};

/*
 * combos
 * ,----------------------------------------.                    ,-----------------------------------------.
 * |      |      |      |      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |      |      |      |      |                    |    BSPC    DEL    '`'     |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |      |      |      |      |-------      -------|      <      :      >      |      |      |
 * |------+------+------+------+------+------|       |    |       |------+------+------+------+------+------|
 * |      |      |     M-x    Spc `   |      |-------|    |-------|      "      '      _      |      |      |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            |      |      |      |      | /       /       \      \  |      |      |      |      |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
 */

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
     * BASE
     * ,----------------------------------------.                    ,-----------------------------------------.
     * |  `   |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  |  `   |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * | ESC  |   Q  |   W  |   E  |   R  |   T  |                    |   Y  |   U  |   I  |   O  |   P  |  '   |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * |LSHIFT|   A  |   S  |   D  |   F  |   G  |-------.    ,-------|   H  |   J  |   K  |   L  |   ;  |RSHIFT|
     * |------+------+------+------+------+------|       |    |       |------+------+------+------+------+------|
     * |LCTRL |   Z  |   X  |   C  |   V  |   B  |-------|    |-------|   N  |   M  |   ,  |   .  |   /  |RCTRL |
     * `-----------------------------------------/       /     \      \-----------------------------------------'
     *            | xxxx | Alt  | Meta |TAB   |SPC(SYMB)/       \SPC(NUMB)| ENT  | BKSPC| XXXX | XXXX |
     *            |      |      |      |      |/       /         \      \ |      |      |      |      |
     *            `----------------------------------'           '------''---------------------------'
     */

    [BASE] = LAYOUT(KC_GRV, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_GRV, KC_ESC, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_QUOT, KC_LSFT, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_RSFT, KC_LCTL, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_MUTE, XXXXXXX, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_RCTL, XXXXXXX, KC_LALT, KC_LGUI, KC_TAB, KC_SYMB_SPC, KC_NUMB_SPC, KC_ENT, KC_BSPC, XXXXXXX, XXXXXXX),
    /* SYMB
     * ,-----------------------------------------.                    ,-----------------------------------------.
     * |      |  F1  |  F2  |  F3  |  F4  |  F5  |                    |  F6  |  F7  |  F8  |  F9  | F10  | F11  |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * | ESC  |   !  |   @  |   {  |   }  |   |  |                    |   `  |   ~  |   \  |      |   "  | F12  |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * |LSHIFT|   #  |   $  |   (  |   )  |  LMB |-------.    ,-------|   +  |   -  |   /  |   *  |   '  |RSHIFT|
     * |------+------+------+------+------+------|  MUTE |    |       |------+------+------+------+------+------|
     * |LCTRL |  %   |  ^   |  [   |   ]  |  RMB |-------|    |-------|   &  |   =  |   ,  |   .  |   _  |RCTRL |
     * `-----------------------------------------/       /     \      \-----------------------------------------'
     *            | xxxx | xxxx | xxxx |xxxxxx| /   =   /       \  =   \  |xxxxx | xxxx | xxxx | xxxx |
     *            |      |      |      |      |/       /         \      \ |      |      |      |      |
     *            `----------------------------------'           '------''---------------------------'
     */
    [SYMB] = LAYOUT(_______, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_ESC, KC_EXLM, KC_AT, KC_LCBR, KC_RCBR, KC_PIPE, KC_GRV, KC_TILD, KC_BSLS, XXXXXXX, KC_DQT, KC_F12, KC_LSFT, KC_HASH, KC_DLR, KC_LPRN, KC_RPRN, KC_BTN2, KC_PLUS, KC_MINS, KC_SLSH, KC_ASTR, KC_QUOT, KC_RSFT, KC_LCTL, KC_PERC, KC_CIRC, KC_LBRC, KC_RBRC, KC_BTN1, _______, _______, KC_AMPR, KC_EQL, KC_COMM, KC_DOT, KC_UNDS, KC_RCTL, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_EQL, KC_EQL, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX),
    /* NUMB
     * ,----------------------------------------.                    ,-----------------------------------------.
     * |      |  F1  |  F2  |  F3  |  F4  |  F5  |                    |  F6  |  F7  |  F8  |  F9  | F10  | F11  |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * | Esc  | Ins  | Pscr | Menu |      |      |                    | HOME | PWrd | NWrd | END  |      | F12  |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * | Tab  | LAt  | LCtl |LShift|      | Caps |-------.    ,-------| LFT  | DOWN | UP   | RIGHT|      |      |
     * |------+------+------+------+------+------|  MUTE |    |       |------+------+------+------+------+------|
     * |LCTRL | Undo |  Cut | Copy | Paste|      |-------|    |-------| PGDN |C(DWN)|C(UP) | PGUP |      |      |
     * `-----------------------------------------/       /     \      \-----------------------------------------'
     *            | xxxx | xxxx | xxxx |xxxxxx| /   =   /       \  =   \  |xxxxx | xxxx | xxxx | xxxx |
     *            |      |      |      |      |/       /         \      \ |      |      |      |      |
     *            `----------------------------------'           '------''---------------------------'
     */
    [NUMB] = LAYOUT(_______, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_ESC, KC_INS, KC_PSCR, KC_APP, XXXXXXX, XXXXXXX, KC_HOME, KC_PRVWD, KC_NXTWD, KC_END, XXXXXXX, KC_F12, KC_TAB, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX, KC_CAPS, KC_LEFT, KC_DOWN, KC_UP, KC_RIGHT, XXXXXXX, XXXXXXX, KC_LCTL, KC_UNDO, KC_CUT, KC_COPY, KC_PASTE, XXXXXXX, _______, _______, KC_PGDN, KC_C_DOWN, KC_C_UP, KC_PGUP, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_EQL, KC_EQL, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX)};

#ifdef OLED_ENABLE

/* static void render_logo_l(void) { */
/*     static const char PROGMEM raw_logo[] = { */
/*         0,  0, 18,128, 32, 10, 64, 18,132, 48,  5, 64, 20,193,  4, 16, 65,  0,132,  0,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,192,  0, 77, 31, 79,183,  5, 79,151, 43,199, 21, 47, 87,239,252,191,244,188,236,120,244,248,192,240,160,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, */
/*         0,  0,  0,170,  0,  0, 21,162,  0,  4, 65,  8,130, 32,140, 65,144, 42,128, 52,128, 36,  0, 73,  0,  0,  0,128,128,  0,192,192,128,  0,  0,  0,  0,  0,  0,  0,  0,128,128,192,224,176,224,242,224,100,160, 68,208,180, 73, 54,192,125,  0,107,132,240,141, 34,  8,230,  8, 85,213,255,255,221,255,255,187,255,255,191,253,119,255,255,126,247,222,124,248,232,192,160,  0,  0, 64,128, 32,  0, 64,144, 32, 64, 80,144,108,  0, 72,144,228,184,104,216,176, 96,128, 96,192,160, 64,128,  0,128,  0,  0,  0,  0,  0,  0,  0,  0,  */
/*         0,128,  0, 10,160,  0,196,240,244,255,255,247,255,191,255,255,247,254,254,254,246,190,255,254,255,247,190,255,255,255,171,255,110,221,246, 61,238,251,214,125,223,243, 94,255,247,190,253,215,127,252,234,179,106,244,253,203, 50,196,253, 63,202,162,212,253,189,226,205,240,215,127,255,238,187,255,109,255, 59,183, 63, 31,173, 15, 43, 67, 11, 83,  1, 87,128, 21,161, 11, 64, 21,192, 21, 41, 66,144,  5, 32, 77,  0, 81,  4, 41,130, 45,  3,254, 37,219,172,115,140,119,200, 63,193,190, 81,175, 84,184, 96,128,  0,  0,  */
/*         0,  0, 32,  4, 84,255,255,255,255,254,255,247,255,255,255,191,142,151,167,135,135, 23,  7, 95, 15, 31, 31, 63,191, 62,251,159,213,223,181, 31, 55, 28, 55, 61, 47,123,255,213,255,191,246,223,253,183,255,255,213,255,255,219,255,191,251,215,255,190,251,239,254,187,255,214,255,189,127,  3, 75,  1, 82,  8, 66, 16, 68,  1, 84,  0, 85,  0, 85,  0, 85,160, 10, 64, 20, 75, 32, 74,  0, 84,  9, 82,  0, 84,137,210,128, 52,129, 40,  2, 80,  4, 81,239,148,107,221, 34,223,104,151,104,223, 32,255,  1,254, 69,190, 72,  0, */
/*     }; */
/*     oled_write_raw_P(raw_logo, sizeof(raw_logo)); */
/* } */

/* static void render_logo_r(void) { */
/*     static const char PROGMEM raw_logo[] = { */
/*         0,  0, 72,128,  1,  7, 47, 15, 71,143,  7,199,223,255,255,127,255,255,255,255,191,254,254,255,190,254,255,254,255,255,255,239, 95,255,251,175,254, 94,254,250,175,253, 95,255,251,111,253,223,119,255,221,255,219,255,247, 95,253,255,107,223,253,255,171,127,253,223,251,191, 39, 14, 80,130,  0, 42,128, 34,  8, 66, 16,138,  0,178,  4, 96, 13, 82,200,181, 42,128, 82,172,  2, 56,194,  8, 50,196, 16, 69,189,  7, 59,202, 10,176,  2, 36, 64, 13,242,157,119,200, 63,210,173, 82,255,128,127,210, 45,210, 47,240, 15,  0, */
/*         0,160,  4,  0,136,  0, 18,  0, 80,246,255,255,255,239,255,223,255,255, 63, 47, 11,  3, 67,  3,  7,  7,  6,  7, 15, 31, 31, 59, 47, 15, 10, 15, 23, 29, 15, 27, 30, 23, 63, 45, 55, 63,125,223,123,247,222,127,247, 62,111,125,255,107,255,223,250,191,235,255, 95,253,255,187,226, 80,138,  0, 40,130,  0,148, 33,  4, 72,  2, 80,132, 17, 36, 83,  4, 82,141, 42,  0, 85, 10, 81,  4,210,  8, 33,194, 12, 17,244, 61, 78,184,  2, 84, 33, 12, 65, 84,255,  0,255,173, 83,254,132,123,172, 83,238, 57,  7, 12, 11,  0,  0,  0, */
/*         0,  0,  4, 64,  0,170,  0, 32,128, 39,141,  7,167, 15,131, 39,  3,136,  1, 16, 66,  8,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,  0,  1,  1,  3,  3,  6,  7, 15,  2,199,255,247,221,255,255,254,218,124,253,250,240,244,232,225,232,  2, 16,  8,  3, 40,  0, 41,  2, 16, 36,  1,  8, 17, 34,  8,  2, 16,  5,  8,  2, 16,  4,  1, 10,  0,  5, 16,  5,  0, 28, 51, 13, 55,  8, 31,  2,  5,  2,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0, */
/*         0,  0,  0, 16,  2, 64, 20, 73,  0, 82,  4, 80,130,  8, 32,  2,128, 20,  0, 32,  0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 29, 63, 95,123,255,126,139, 63, 79,155, 55, 79,190,251,111,255, 42,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, */
/*     }; */
/*     oled_write_raw_P(raw_logo, sizeof(raw_logo)); */
/* } */

// static void print_status_narrow(void) {
//  Print current mode
//  oled_write_P(PSTR("\n\n"), false);
//  oled_write_ln_P(PSTR("Hi"), false);
//}

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    // if (is_keyboard_master()) {
    //    return OLED_ROTATION_270;
    //}
    return rotation;
}

static void render_buffer(void) { oled_write_raw((char *)screen_buf, sizeof(screen_buf)); }

static bool should_render = false;

void oled_task_user(void) {
    if (should_render) {
        render_buffer();
        should_render = false;
    }
    /* if (is_keyboard_master()) { */
    /*     render_logo_l(); */
    /* } else { */
    /*     render_logo_r(); */
    /* } */
}

struct __attribute__((__packed__)) display_packet {
    uint16_t offset;
    uint16_t length;
    bool     to_master;
    uint8_t  buf[];
};

void copy_display_pkt(struct display_packet *pkt, uint8_t length) {
    /* if (((size_t)pkt->length) > ((size_t)length - sizeof(struct display_packet))) { */
    /*     /\* uprintf("Packet noped\n"); *\/ */
    /*     return; */
    /* } */

    /* uprintf("Writing into buf at %d %d bytes\n", pkt->offset, pkt->length); */

    if ((pkt->length + pkt->offset) > sizeof(screen_buf)) return;

    memcpy(&screen_buf[pkt->offset], pkt->buf, pkt->length);
}

void raw_hid_receive(uint8_t *data, uint8_t length) {
    /* uprintf("data: %d %d %d %d %d %d %d\n",data[0],data[1],data[2],data[3],data[4],data[5], data[6]); */
    switch (*data) {
        case 0:
            break;
        case 1: {
            struct display_packet *pkt = (struct display_packet *)&data[1];
            /* uprintf("to_master: %d\n", pkt->to_master); */
            if (pkt->to_master) {
                copy_display_pkt(pkt, length - 1);
            } else {
                for (int i = 0; i < 4; i++) {
                    if (transaction_rpc_send(USER_SEND_DISP, length - 1, pkt)) {
                        break;
                    }
                }
            }

            break;
        }
        case 2: {
            should_render = true;
            int x         = 0;
            for (int i = 0; i < 4; i++) {
                if (transaction_rpc_send(USER_FLIP_DISP, sizeof(x), &x)) {
                    break;
                }
            }
            break;
        }
        default:
            break;
    }
    return false;
}

void user_send_disp_handler(uint8_t length, const void *data, uint8_t out_len, void *out_data) {
    /* uprintf("whoa from slave\n"); */
    if (length < sizeof(struct display_packet)) return;

    struct display_packet *pkt = (struct display_packet *)data;

    copy_display_pkt(pkt, length);
}

void user_flip_disp_handler(uint8_t length, const void *data, uint8_t out_len, void *out_data) { should_render = true; }

void keyboard_post_init_user(void) {
    transaction_register_rpc(USER_SEND_DISP, user_send_disp_handler);
    transaction_register_rpc(USER_FLIP_DISP, user_flip_disp_handler);
}

#endif

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case KC_PRVWD:
            if (record->event.pressed) {
                if (keymap_config.swap_lctl_lgui) {
                    register_mods(mod_config(MOD_LALT));
                    register_code(KC_LEFT);
                } else {
                    register_mods(mod_config(MOD_LCTL));
                    register_code(KC_LEFT);
                }
            } else {
                if (keymap_config.swap_lctl_lgui) {
                    unregister_mods(mod_config(MOD_LALT));
                    unregister_code(KC_LEFT);
                } else {
                    unregister_mods(mod_config(MOD_LCTL));
                    unregister_code(KC_LEFT);
                }
            }
            break;
        case KC_NXTWD:
            if (record->event.pressed) {
                if (keymap_config.swap_lctl_lgui) {
                    register_mods(mod_config(MOD_LALT));
                    register_code(KC_RIGHT);
                } else {
                    register_mods(mod_config(MOD_LCTL));
                    register_code(KC_RIGHT);
                }
            } else {
                if (keymap_config.swap_lctl_lgui) {
                    unregister_mods(mod_config(MOD_LALT));
                    unregister_code(KC_RIGHT);
                } else {
                    unregister_mods(mod_config(MOD_LCTL));
                    unregister_code(KC_RIGHT);
                }
            }
            break;
        case KC_COPY:
            if (record->event.pressed) {
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_C);
            } else {
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_C);
            }
            return false;
        case KC_PASTE:
            if (record->event.pressed) {
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_V);
            } else {
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_V);
            }
            return false;
        case KC_CUT:
            if (record->event.pressed) {
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_X);
            } else {
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_X);
            }
            return false;
            break;
        case KC_UNDO:
            if (record->event.pressed) {
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_Z);
            } else {
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_Z);
            }
            return false;
    }
    return true;
}

#ifdef ENCODER_ENABLE

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) {
        if (clockwise) {
            tap_code(KC_VOLU);
        } else {
            tap_code(KC_VOLD);
        }
    } else if (index == 1) {
        if (clockwise) {
            tap_code(KC_PGDOWN);
        } else {
            tap_code(KC_PGUP);
        }
    }
    return true;
}

#endif
