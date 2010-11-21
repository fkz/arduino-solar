PROJECT(fernbedienung)

add_executable (fernbedienung flags.cpp main.cpp ../wireless.cpp fernbedienung.cpp Menu.cpp solarboot_data.cpp lcd_helper.cpp pot_value.cpp)
target_link_libraries(fernbedienung -Wl,--gc-sections arduinoLib Lcd EEPROM MemoryManagement)

ADD_CUSTOM_COMMAND(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/upload.hex
  COMMAND avr-objcopy -O ihex -R .eeprom ${CMAKE_CURRENT_BINARY_DIR}/fernbedienung ${CMAKE_CURRENT_BINARY_DIR}/upload.hex 
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/fernbedienung)

add_custom_target(generate-fernbedienung

DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/upload.hex)

if (avrdudePort)
add_custom_target(upload-fernbedienung
 ${avrdude} -C${avrdudeConf} -v -patmega328p -cstk500v1 -P${avrdudePort} -b57600 -D -Uflash:w:${CMAKE_CURRENT_BINARY_DIR}/upload.hex:i 
DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/upload.hex)
else (avrdudePort)
add_custom_target(upload-fernbedienung
 ${avrdude} -C${avrdudeConf} -v -patmega328p -cstk500v1 -P`ls /dev/ttyUSB*` -b57600 -D -Uflash:w:${CMAKE_CURRENT_BINARY_DIR}/upload.hex:i 
DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/upload.hex)
endif (avrdudePort)