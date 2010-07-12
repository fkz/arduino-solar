PROJECT(project)

set (path ${arduinoDirectory}/hardware/arduino/cores/arduino)
add_executable (solarTest main.cpp)
target_link_libraries(solarTest -Wl,--gc-sections arduinoLib EEPROM)

ADD_CUSTOM_COMMAND(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/upload.hex
  COMMAND avr-objcopy -O ihex -R .eeprom ${CMAKE_CURRENT_BINARY_DIR}/solarTest ${CMAKE_CURRENT_BINARY_DIR}/upload.hex 
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/solarTest)

add_custom_target(upload 
 avrdude -V -c stk500v1 -p m168 -b 19200 -P /dev/ttyUSB0 -U flash:w:${CMAKE_CURRENT_BINARY_DIR}/upload.hex  
 DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/upload.hex)
