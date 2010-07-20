PROJECT(fernbedienung)

add_executable (fernbedienung main.cpp ../wireless.cpp fernbedienung.cpp)
target_link_libraries(fernbedienung -Wl,--gc-sections arduinoLib Lcd XBee)

ADD_CUSTOM_COMMAND(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/upload.hex
  COMMAND avr-objcopy -O ihex -R .eeprom ${CMAKE_CURRENT_BINARY_DIR}/fernbedienung ${CMAKE_CURRENT_BINARY_DIR}/upload.hex 
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/fernbedienung)

add_custom_target(upload-fernbedienung
 avrdude -V -c stk500v1 -p m168 -b 19200 -P usb -U flash:w:${CMAKE_CURRENT_BINARY_DIR}/upload.hex  
 DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/upload.hex)
