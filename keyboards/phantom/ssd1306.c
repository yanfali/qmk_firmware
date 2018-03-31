#include "i2cmaster.h"
#include "ssd1306.h"

void ssd1306_init(void)
{
  i2c_init();
  ssd1306_command(SSD1306_DISPLAYOFF);
  ssd1306_command(SSD1306_SETDISPLAYCLOCKDIV);
  ssd1306_command(0x80);

  ssd1306_command(SSD1306_SETMULTIPLEX);
  ssd1306_command(SSD1306_LCDHEIGHT - 1);

  ssd1306_command(SSD1306_SETDISPLAYOFFSET);              // 0xD3
  ssd1306_command(0x0);                                   // no offset
  ssd1306_command(SSD1306_SETSTARTLINE | 0x0);            // line #0
  ssd1306_command(SSD1306_CHARGEPUMP);                    // 0x8D

  ssd1306_command(0x14); // internal VCC
  ssd1306_command(SSD1306_MEMORYMODE);                    // 0x20
  ssd1306_command(0x00);                                  // 0x0 act like ks0108
  ssd1306_command(SSD1306_SEGREMAP | 0x1);
  ssd1306_command(SSD1306_COMSCANDEC);

  ssd1306_command(SSD1306_SETCOMPINS);                    // 0xDA
  ssd1306_command(0x12);
  ssd1306_command(SSD1306_SETCONTRAST);                   // 0x81

  ssd1306_command(0xCF); // internal VCC

  ssd1306_command(SSD1306_SETPRECHARGE);                  // 0xd9
  ssd1306_command(0xF1); // internal VCC
  ssd1306_command(SSD1306_SETVCOMDETECT);                 // 0xDB
  ssd1306_command(0x40);
  ssd1306_command(SSD1306_DISPLAYALLON_RESUME);           // 0xA4
  ssd1306_command(SSD1306_NORMALDISPLAY);                 // 0xA6

  ssd1306_command(SSD1306_DEACTIVATE_SCROLL);

  ssd1306_command(SSD1306_DISPLAYON);//--turn on oled panel

}

uint8_t ssd1306_command(uint8_t c) {
  uint8_t control = 0x00; // Co = 0, D/C = 0
  uint8_t ret;
  ret = i2c_start(SSD1306_I2C_ADDRESS);
  if (ret) goto stop;
  ret = i2c_write(control);
  if (ret) goto stop;
  ret = i2c_write(c);

 stop:
  i2c_stop();
  return ret;
}
