PROJECT(fernbedienung)

add_executable (fernbedienung main.cpp ../wireless.cpp fernbedienung.cpp)
target_link_libraries(fernbedienung -Wl,--gc-sections arduinoLib Lcd XBee)

ADD_CUSTOM_COMMAND(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/upload.hex
  COMMAND avr-objcopy -O ihex -R .eeprom ${CMAKE_CURRENT_BINARY_DIR}/fernbedienung ${CMAKE_CURRENT_BINARY_DIR}/upload.hex 
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/fernbedienung)

add_custom_target(upload-fernbedienung
 ${arduinoDirectory}/hardware/tools/avrdude -C${arduinoDirectory}/hardware/tools/avrdude.conf -v -patmega328p -cstk500v1 -P`ls /dev/ttyUSB*` -b57600 -D -Uflash:w:${CMAKE_CURRENT_BINARY_DIR}/upload.hex:i 
DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/upload.hex)