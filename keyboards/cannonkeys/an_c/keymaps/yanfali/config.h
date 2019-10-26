
#undef EEPROM_MAGIC_ADDR
#define EEPROM_MAGIC_ADDR 40
// Bump this every time we change what we store	// Bump this every time we change what we store
// This will automatically reset the EEPROM with defaults	// This will automatically reset the EEPROM with defaults
// and avoid loading invalid data from the EEPROM	// and avoid loading invalid data from the EEPROM
#undef EEPROM_VERSION
#define EEPROM_VERSION 0x02
#undef EEPROM_VERSION_ADDR
#define EEPROM_VERSION_ADDR 42

#undef DYNAMIC_KEYMAP_LAYER_COUNT
#define DYNAMIC_KEYMAP_LAYER_COUNT 4
// Dynamic macro starts after dynamic keymaps (35+(4*5*15*2)) = (35+600) = 635	// Dynamic macro starts after dynamic keymaps (35+(4*5*15*2)) = (35+600) = 635
// start + layer * rows * col * 2	// start + layer * rows * col * 2
#undef DYNAMIC_KEYMAP_EEPROM_ADDR
#undef EEPROM_CUSTOM_BACKLIGHT
#undef DYNAMIC_KEYMAP_MACRO_EEPROM_ADDR
#undef DYNAMIC_KEYMAP_MACRO_EEPROM_SIZE
#undef DYNAMIC_KEYMAP_MACRO_COUNT
#define DYNAMIC_KEYMAP_EEPROM_ADDR 43
#define EEPROM_CUSTOM_BACKLIGHT 644
#define DYNAMIC_KEYMAP_MACRO_EEPROM_ADDR 645
#define DYNAMIC_KEYMAP_MACRO_EEPROM_SIZE 192
#define DYNAMIC_KEYMAP_MACRO_COUNT 16
