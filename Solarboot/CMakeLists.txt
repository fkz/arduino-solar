PROJECT(project)

add_executable (solarboot main.cpp solarboot.cpp ../wireless.cpp)
target_link_libraries(solarboot -Wl,--gc-sections arduinoLib Servo)

ADD_CUSTOM_COMMAND(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/upload.hex
  COMMAND avr-objcopy -O ihex -R .eeprom ${CMAKE_CURRENT_BINARY_DIR}/solarboot ${CMAKE_CURRENT_BINARY_DIR}/upload.hex 
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/solarboot)

add_custom_target(upload-solarboot
 ${arduinoDirectory}/hardware/tools/avrdude -C${arduinoDirectory}/hardware/tools/avrdude.conf -v -patmega328p -cstk500v1 -P`ls /dev/ttyUSB*` -b57600 -D -Uflash:w:${CMAKE_CURRENT_BINARY_DIR}/upload.hex:i 
 DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/upload.hex)
