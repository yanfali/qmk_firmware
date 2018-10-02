/*
Copyright 2012 Jun Wako <wakojun@gmail.com>
Copyright 2015 Jack Humbert

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

#pragma once

#include QMK_KEYBOARD_CONFIG_H

#define C6_AUDIO
#define AUDIO_VOICES
#define AUDIO_CLICKY
#define AUDIO_CLICKY_FREQ_RANDOMNESS 1.0f

#define STARTUP_SONG SONG(ZELDA_PUZZLE)

#undef MATRIX_ROW_PINS
#undef MATRIX_COL_PINS
#define MATRIX_ROW_PINS { F4, D4, D7, E6, B4, D6 }
#define MATRIX_COL_PINS { F5, F6, F7, B1, B3, B2, B6, C6 }
#define MATRIX_ROW_PINS_RIGHT { F4, D4, D7, E6, B4, B5 }
#define MATRIX_COL_PINS_RIGHT { F5, F6, F7, B1, B3, B2, B6, C6 }

//#define USE_I2C
//#undef USE_SERIAL
