BOARD_TAG = atmega328
ARDUINO_LIBS += FastLED Encoder

MONITOR_PORT = /dev/ttyUSB*

include $(ARDMK_DIR)/Arduino.mk
