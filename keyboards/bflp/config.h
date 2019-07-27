/*
Copyright 2019 Yan-Fa Li

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

#include "config_common.h"

/* USB Device descriptor parameter */
#define VENDOR_ID       0x1337
#define PRODUCT_ID      0xbf7d
#define DEVICE_VER      0x0001
#define MANUFACTURER    Maple Computing
#define PRODUCT         BFLP
#define DESCRIPTION     "An all-in-on big f'in macropad"

/* key matrix size */
#define MATRIX_ROWS 4
#define MATRIX_COLS 2

/*
 * Keyboard Matrix Assignments
 *
 * Change this to how you wired your keyboard
 * COLS: AVR pins used for columns, left to right
 * ROWS: AVR pins used for rows, top to bottom
 * DIODE_DIRECTION: COL2ROW = COL = Anode (+), ROW = Cathode (-, marked on diode)
 *                  ROW2COL = ROW = Anode (+), COL = Cathode (-, marked on diode)
 *
*/
#define DIRECT_PINS { \
    { F4, D1, }, \
    { F5, D0, }, \
    { F6, D4, }, \
    { F7, C6, } \
}

/* COL2ROW, ROW2COL*/
#define DIODE_DIRECTION COL2ROW

