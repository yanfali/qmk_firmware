#include <stdbool.h>
#include "action.h"
#include "i2cmaster.h"
#include "expander.h"
#include "debug.h"

static uint8_t expander_status = 0;

void expander_config(void);
uint8_t expander_write(uint8_t reg, uint8_t data);
uint8_t expander_read(uint8_t reg, uint8_t *data);

void expander_init(void)
{
  expander_scan();
}

void expander_scan(void)
{
  dprintf("expander status: %d ... ", expander_status);
  uint8_t ret = i2c_start(EXPANDER_ADDR | I2C_WRITE);
  if (ret == 0) {
    i2c_stop();
    if (expander_status == 0) {
      dprintf("attached\n");
      expander_status = 1;
      expander_config();
      clear_keyboard();
    }
  }
  else {
    if (expander_status == 1) {
      dprintf("detached\n");
      expander_status = 0;
      clear_keyboard();
    }
  }
  dprintf("%d\n", expander_status);
}

void expander_select_col(uint8_t col)
{
  uint8_t gpioa = 0xFF;
  uint8_t colp = col - (MATRIX_COLS - 8);
  gpioa &= ~(1<<colp); // all set to 0
  // dprintf("gpioa %d col %d colp %d\n", gpioa, col, colp);

  expander_write(EXPANDER_REG_IODIRA, gpioa); // OUT
  expander_write(EXPANDER_REG_GPIOA, gpioa); // HI
}

void expander_unselect_cols()
{
  expander_write(EXPANDER_REG_IODIRA, 0xFF); // IN
  expander_write(EXPANDER_REG_GPIOA, 0xFF); // HI
}

void expander_config(void)
{
  expander_write(EXPANDER_REG_IODIRA, 0xFF); // input
  expander_write(EXPANDER_REG_GPIOA, 0x00); // all set to 0

  // LEDs
  expander_write(EXPANDER_REG_IODIRB, 0x00); // output
  expander_write(EXPANDER_REG_GPIOB, 0x00); // all set to 0
}

void expander_toggle_led(void) {
  uint8_t gpiob = 0;
  expander_read(EXPANDER_REG_GPIOB, &gpiob);

  dprintf("gpiob %d\n", gpiob);
  gpiob ^= (1<<3);
  expander_write(EXPANDER_REG_GPIOB, gpiob);
}

uint8_t expander_write(uint8_t reg, uint8_t data)
{
  if (expander_status == 0) {
    return 0;
  }
  uint8_t ret;
  ret = i2c_start(EXPANDER_ADDR | I2C_WRITE);
  if (ret) goto stop;
  ret = i2c_write(reg);
  if (ret) goto stop;
  ret = i2c_write(data);
 stop:
  i2c_stop();
  return ret;
}

uint8_t expander_read(uint8_t reg, uint8_t *data)
{
  if (expander_status == 0) {
    return 0;
  }
  uint8_t ret;
  ret = i2c_start(EXPANDER_ADDR | I2C_WRITE);
  if (ret) goto stop;
  ret = i2c_write(reg);
  if (ret) goto stop;
  ret = i2c_rep_start(EXPANDER_ADDR | I2C_READ);
  if (ret) goto stop;
  *data = i2c_readNak();
 stop:
  i2c_stop();
  return ret;
}
