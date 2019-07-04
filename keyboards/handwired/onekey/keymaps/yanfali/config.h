/* Copyright 2019
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "config_common.h"

#undef MATRIX_COLS
#undef MATRIX_ROWS

#define MATRIX_COLS 5
#define MATRIX_ROWS 5

#undef MATRIX_COL_PINS
#undef MATRIX_ROW_PINS

#define MATRIX_COL_PINS { B9, B8, B7, B6, B5 }
#define MATRIX_ROW_PINS { A8, B15, B14, B13, B12 }
#define UNUSED_PINS
