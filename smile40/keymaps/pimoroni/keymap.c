// Copyright 2023 %YOUR_FULL_NAME% (@%YOUR_GITHUB_USERNAME%)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

enum layer_names {
    _BASE,
    _LOWER,
    _RAISE,
    _ADJUST
};

enum custom_keycodes {
  BALL_HUI = SAFE_RANGE, //cycles hue
  BALL_WHT,              //cycles white
  BALL_DEC,              //decreased color
  BALL_SCR,              //scrolls
  BALL_NCL,              //left click
  BALL_RCL,              //right click
  BALL_MCL,              //middle click
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        
        KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_DEL,
        KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT, KC_ENT,
        LSFT_T(KC_Z),KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, RSFT_T(KC_SLSH), KC_UP, KC_ESC,
        KC_LCTL, KC_LGUI,    LT(1,KC_BSPC), LT(2,KC_SPC),   LT(3,KC_RALT),       KC_LEFT, KC_DOWN, KC_RGHT 
             
    ),

    [1] = LAYOUT(
        
        KC_TILD, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0,
        KC_TAB, KC_BTN3, KC_BTN2, KC_BTN1, BALL_SCR, KC_MINS, KC_EQL, KC_LBRC, KC_RBRC, KC_GRV, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_BSLS, _______, _______,
        _______, _______,          _______, _______,          _______,                   _______, _______, _______ 
    ),

    [2] = LAYOUT(
        
        KC_EXLM, KC_AT, KC_HASH, KC_DLR, KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, _______, 
        _______, _______, _______, _______, _______, _______, _______, _______,  _______, _______, _______, _______, 
        _______, _______, KC_MUTE, KC_VOLU, KC_VOLD, RGB_TOG, RGB_MOD, RGB_HUI, RGB_SAI, RGB_VAI, _______, _______,
        _______, _______,          _______, _______,          _______,                   BALL_HUI, BALL_WHT, BALL_DEC 
    ),

    [3] = LAYOUT(
        KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11,
        KC_F12, KC_F13, KC_F14, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______,  _______,  _______, _______, _______,  _______,  _______, _______, _______, _______,  _______,
        _______, _______,          _______,  _______,         _______,                   _______,  _______,  _______ 
         
    )
};
uint8_t white = 0;
uint8_t red = 255;
uint8_t green = 0;
uint8_t blue = 0;

bool set_scrolling = false;
report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    if (set_scrolling) {
        mouse_report.h = mouse_report.x;
        mouse_report.v = mouse_report.y;
        mouse_report.x = mouse_report.y = 0; 
    }
    return mouse_report;
}

void ball_increase_hue(void){
      if(red!=255&&green!=255&&blue!=255){
        red =255;
      }
      if (red==255&&green<255&&blue==0){
       green += 15;
      } else if(green==255&&blue==0&&red>0){
        red-=15;
      } else if(red==0&&blue<255&&green==255){
        blue+=15;
      } else if(blue==255&&green>0&&red==0){
        green -= 15;
      } else if(green == 0&&blue==255&&red<255){
        red +=15;
      } else if(green ==0&&blue>0&&red==255){
        blue -=15;
      }
      pimoroni_trackball_set_rgbw(red,green,blue,white);
}

void decrease_color(void){
  if (green>0){
    green-=15;
  }
  if (red>0){
    red-=15;
  }
  if (blue>0){
    blue-=15;
  }
  pimoroni_trackball_set_rgbw(red,green,blue,white);
}

void cycle_white(void){
  if (white<255){
    white +=15;
  } else{
    white=0;
  }
  pimoroni_trackball_set_rgbw(red,green,blue,white);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record){
  switch (keycode){
  case  BALL_HUI:
    if(record->event.pressed){
      ball_increase_hue();
    }
    break;

  case BALL_WHT:
    if(record-> event.pressed){
      cycle_white();
    }
    break;

  case BALL_DEC:
   if(record-> event.pressed){
      decrease_color();
    }
    break;

  case BALL_SCR:
   if(record->event.pressed){
     set_scrolling = true;
   } else{
     set_scrolling = false;
   }
   break;

  case BALL_NCL:
     record->event.pressed?register_code(KC_BTN1):unregister_code(KC_BTN1);
     break;
  case BALL_RCL:
      record->event.pressed?register_code(KC_BTN2):unregister_code(KC_BTN2);
      break;
  case BALL_MCL:
      record->event.pressed?register_code(KC_BTN3):unregister_code(KC_BTN3);
      break;
  }
  return true;
}
