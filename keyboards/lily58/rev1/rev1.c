#include "lily58.h"
#include "rev1/ssd1306.h"

#ifdef SSD1306OLED
void led_set_kb(uint8_t usb_led) {
    // put your keyboard LED indicator (ex: Caps Lock LED) toggling code here
    //led_set_user(usb_led);
}
bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
  return process_record_gfx(keycode,record) && process_record_user(keycode, record);
}
#endif
