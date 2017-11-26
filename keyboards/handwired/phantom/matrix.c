/*

Note for ErgoDox EZ customizers: Here be dragons!
This is not a file you want to be messing with.
All of the interesting stuff for you is under keymaps/ :)
Love, Erez

Copyright 2013 Oleg Kostyuk <cub.uanic@gmail.com>

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

/*
 * scan matrix
 */
#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include "wait.h"
#include "action_layer.h"
#include "print.h"
#include "debug.h"
#include "util.h"
#include "matrix.h"
#include QMK_KEYBOARD_H
#include "i2cmaster.h"
#include "ergodox_ez.h"
#ifdef DEBUG_MATRIX_SCAN_RATE
#include  "timer.h"
#endif

/*
 * This constant define not debouncing time in msecs, but amount of matrix
 * scan loops which should be made to get stable debounced results.
 *
 * On Ergodox matrix scan rate is relatively low, because of slow I2C.
 * Now it's only 317 scans/second, or about 3.15 msec/scan.
 * According to Cherry specs, debouncing time is 5 msec.
 *
 * And so, there is no sense to have DEBOUNCE higher than 2.
 */

#ifndef DEBOUNCE
#   define DEBOUNCE	5
#endif

/* matrix state(1:on, 0:off) */
static matrix_row_t matrix[MATRIX_ROWS];

// Debouncing: store for each key the number of scans until it's eligible to
// change.  When scanning the matrix, ignore any changes in keys that have
// already changed in the last DEBOUNCE scans.
static uint8_t debounce_matrix[MATRIX_ROWS * MATRIX_COLS];

static matrix_row_t read_cols(uint8_t row);
static void init_cols(void);
static void unselect_rows(void);
static void select_row(uint8_t row);

static uint8_t mcp23018_reset_loop;

#ifdef DEBUG_MATRIX_SCAN_RATE
uint32_t matrix_timer;
uint32_t matrix_scan_count;
#endif


__attribute__ ((weak))
void matrix_init_user(void) {}

__attribute__ ((weak))
void matrix_scan_user(void) {}

__attribute__ ((weak))
void matrix_init_kb(void) {
  matrix_init_user();
}

__attribute__ ((weak))
void matrix_scan_kb(void) {
  matrix_scan_user();
}

inline
uint8_t matrix_rows(void)
{
    return MATRIX_ROWS;
}

inline
uint8_t matrix_cols(void)
{
    return MATRIX_COLS;
}

void matrix_init(void)
{
    // initialize row and col

    mcp23018_status = init_mcp23018();


    unselect_rows();
    init_cols();

    // initialize matrix state: all keys off
    for (uint8_t i=0; i < MATRIX_ROWS; i++) {
        matrix[i] = 0;
        for (uint8_t j=0; j < MATRIX_COLS; ++j) {
            debounce_matrix[i * MATRIX_COLS + j] = 0;
        }
    }

#ifdef DEBUG_MATRIX_SCAN_RATE
    matrix_timer = timer_read32();
    matrix_scan_count = 0;
#endif

    matrix_init_quantum();

}

void matrix_power_up(void) {
    mcp23018_status = init_mcp23018();

    unselect_rows();
    init_cols();

    // initialize matrix state: all keys off
    for (uint8_t i=0; i < MATRIX_ROWS; i++) {
        matrix[i] = 0;
    }

#ifdef DEBUG_MATRIX_SCAN_RATE
    matrix_timer = timer_read32();
    matrix_scan_count = 0;
#endif
}

// Returns a matrix_row_t whose bits are set if the corresponding key should be
// eligible to change in this scan.
matrix_row_t debounce_mask(uint8_t row) {
  matrix_row_t result = 0;
  for (uint8_t j=0; j < MATRIX_COLS; ++j) {
    if (debounce_matrix[row * MATRIX_COLS + j]) {
      --debounce_matrix[row * MATRIX_COLS + j];
    } else {
      result |= (1 << j);
    }
  }
  return result;
}

// Report changed keys in the given row.  Resets the debounce countdowns
// corresponding to each set bit in 'change' to DEBOUNCE.
void debounce_report(matrix_row_t change, uint8_t row) {
  for (uint8_t i = 0; i < MATRIX_COLS; ++i) {
    if (change & (1 << i)) {
      debounce_matrix[row * MATRIX_COLS + i] = DEBOUNCE;
    }
  }
}

uint8_t matrix_scan(void)
{
    if (mcp23018_status) { // if there was an error
        if (++mcp23018_reset_loop == 0) {
            // since mcp23018_reset_loop is 8 bit - we'll try to reset once in 255 matrix scans
            // this will be approx bit more frequent than once per second
            print("trying to reset mcp23018\n");
            mcp23018_status = init_mcp23018();
            if (mcp23018_status) {
                print("left side not responding\n");
            } else {
                print("left side attached\n");
            }
        }
    }

#ifdef DEBUG_MATRIX_SCAN_RATE
    matrix_scan_count++;

    uint32_t timer_now = timer_read32();
    if (TIMER_DIFF_32(timer_now, matrix_timer)>1000) {
        print("matrix scan frequency: ");
        pdec(matrix_scan_count);
        print("\n");

        matrix_timer = timer_now;
        matrix_scan_count = 0;
    }
#endif

    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        select_row(i);
        wait_us(30);  // without this wait read unstable value.
        matrix_row_t mask = debounce_mask(i);
        matrix_row_t cols = (read_cols(i) & mask) | (matrix[i] & ~mask);
        debounce_report(cols ^ matrix[i], i);
        matrix[i] = cols;

        unselect_rows();
    }

    matrix_scan_quantum();

    return 1;
}

bool matrix_is_modified(void) // deprecated and evidently not called.
{
    return true;
}

inline
bool matrix_is_on(uint8_t row, uint8_t col)
{
    return (matrix[row] & ((matrix_row_t)1<<col));
}

inline
matrix_row_t matrix_get_row(uint8_t row)
{
    return matrix[row];
}

void matrix_print(void)
{
    print("\nr/c 0123456789ABCDEF\n");
    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        phex(row); print(": ");
        pbin_reverse16(matrix_get_row(row));
        print("\n");
    }
}

uint8_t matrix_key_count(void)
{
    uint8_t count = 0;
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        count += bitpop16(matrix[i]);
    }
    return count;
}

/* Column pin configuration
 *
 * Teensy
 * col: 0   1   2   3   4   5
 * pin: F0  F1  F4  F5  F6  F7
 *
 */
static void  init_cols(void)
{
    // init on mcp2301X
    // not needed, already done as part of init_mcp23018()

    // init on bluefruit
    // Input with pull-up(DDR:0, PORT:1)
    DDRC  &= ~(1<<7 | 1<<6);
    PORTC |= ~(1<<7 | 1<<6);
    DDRD  &= ~(1<<6 | 1<<7 | 1<<2 | 1<<1);
    PORTD |= ~(1<<6 | 1<<7 | 1<<2 | 1<<1);
    DDRB  &= ~(1<<7 | 1<<6 | 1<<5);
    PORTB |= ~(1<<7 | 1<<6 | 1<<5);
}

static matrix_row_t read_cols(uint8_t row)
{
    // I need to read the i2c and combine it with the bluefruit pins
    // 8 bits come from the GPIOA and the 9 other bits from the bluefruit.

    uint8_t data = 0;
    // This uses the row to figure out which thing to read.
    if (!mcp23018_status) { // if there was an error
        mcp23018_status = i2c_start(I2C_ADDR_WRITE);    if (mcp23018_status) goto out;
        mcp23018_status = i2c_write(GPIOA);             if (mcp23018_status) goto out;
        mcp23018_status = i2c_start(I2C_ADDR_READ);     if (mcp23018_status) goto out;
        data = i2c_readNak();
        data = ~data;
    out:
        i2c_stop();
    }

    // read from bluefruit and combine 8 bits from mcp23017/18
    return
        (PINC&(1<<7) ? 0 : (1<<7)) |
        (PIND&(1<<6) ? 0 : (1<<6)) |
        (PINB&(1<<7) ? 0 : (1<<7)) |
        (PINB&(1<<6) ? 0 : (1<<6)) |
        (PINB&(1<<5) ? 0 : (1<<5)) |
        (PIND&(1<<7) ? 0 : (1<<7)) |
        (PINC&(1<<6) ? 0 : (1<<6)) |
        (PIND&(1<<2) ? 0 : (1<<2)) |
        (PIND&(1<<1) ? 0 : (1<<1)) |
        data;
}

/* Row pin configuration
 *
 * Bluefruit
 * row: 0   1   2   3   4   5
 * pin: F7  F6  F5  F4  F1  F0
 *
 */
static void unselect_rows(void)
{
    // unselect on mcp23018
    if (mcp23018_status) { // if there was an error
        // do nothing
    } else {
        // set all rows hi-Z : 1
        mcp23018_status = i2c_start(I2C_ADDR_WRITE);    if (mcp23018_status) goto out;
        mcp23018_status = i2c_write(GPIOA);             if (mcp23018_status) goto out;
        mcp23018_status = i2c_write( 0xFF
                              & ~(0<<7)
                          );                            if (mcp23018_status) goto out;
    out:
        i2c_stop();
    }

    // unselect on bluefruit
    // Hi-Z(DDR:0, PORT:0) to unselect
    DDRC  &= ~(1<<7 | 1<<6);
    PORTC &= ~(1<<7 | 1<<6);
    DDRD  &= ~(1<<6 | 1<<7 | 1<<2 | 1<<1);
    PORTD &= ~(1<<6 | 1<<7 | 1<<2 | 1<<1);
    DDRB  &= ~(1<<7 | 1<<6 | 1<<5);
    PORTB &= ~(1<<7 | 1<<6 | 1<<5);
}

static void select_row(uint8_t row)
{
    // select on bluefruit
    // Output low(DDR:1, PORT:0) to select
    switch (row) {
        case 0:
            DDRF  |= (1<<7);
            PORTB &= ~(1<<7);
            break;
        case 1:
            DDRB  |= (1<<6);
            PORTB &= ~(1<<6);
            break;
        case 2:
            DDRB  |= (1<<5);
            PORTB &= ~(1<<5);
            break;
        case 3:
            DDRB  |= (1<<4);
            PORTB &= ~(1<<4);
            break;
        case 4:
            DDRD  |= (1<<1);
            PORTD &= ~(1<<1);
        break;
        case 5:
            DDRD  |= (1<<0);
            PORTD &= ~(1<<0);
            break;
    }
}

