RGBLIGHT_ENABLE = no
AUDIO_ENABLE = yes
MOUSEKEY_ENABLE = no
EXTRAFLAGS += -flto

ifndef QUANTUM_DIR
	include ../../../../Makefile
endif
