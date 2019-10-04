/*
Copyright 2017 Danny Nguyen <danny@hexwire.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CONFIG_USER_H
#define CONFIG_USER_H

#include "config_common.h"

/* Use I2C or Serial, not both */



//#define USE_SERIAL
#define USE_I2C

// #define TAPPING_FORCE_HOLD
#ifdef TAPPING_TERM
#undef TAPPING_TERM
#endif

#define TAPPING_TERM 150
#define RETRO_TAPPING


/* Select hand configuration */

#define MASTER_LEFT
// #define MASTER_RIGHT
// #define EE_HANDS

#undef RGBLED_NUM
#define RGBLIGHT_ANIMATIONS
#define RGBLED_NUM 12
#define RGBLIGHT_HUE_STEP 8
#define RGBLIGHT_SAT_STEP 8
#define RGBLIGHT_VAL_STEP 8

#define PREVENT_STUCK_MODIFIERS

#ifdef AUDIO_ENABLE
#define C6_AUDIO
//#define STARTUP_SONG SONG(CLOSE_ENCOUNTERS_5_NOTE)
#define AUDIO_CLICKY
#define AUDIO_CLICKY_FREQ_RANDOMNESS 1.0f
#endif

#endif
