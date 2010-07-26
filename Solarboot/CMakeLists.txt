PROJECT(project)

add_executable (solarboot main.cpp solarboot.cpp ../wireless.cpp)
target_link_libraries(solarboot -Wl,--gc-sections arduinoLib Servo)

ADD_CUSTOM_COMMAND(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/upload.hex
  COMMAND avr-objcopy -O ihex -R .eeprom ${CMAKE_CURRENT_BINARY_DIR}/solarboot ${CMAKE_CURRENT_BINARY_DIR}/upload.hex 
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/solarboot)

add_custom_target(upload-solarboot
 avrdude -V -c stk500v1 -p m168 -b 19200 -P usb -U flash:w:${CMAKE_CURRENT_BINARY_DIR}/upload.hex  
 DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/upload.hex)
