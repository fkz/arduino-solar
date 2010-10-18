PROJECT(project)

add_definitions(-DOLD_CASE)

add_executable (solarboot main.cpp solarboot.cpp mppt.cpp ../wireless.cpp)
target_link_libraries(solarboot -Wl,--gc-sections arduinoLib Servo FreqCounter)

ADD_CUSTOM_COMMAND(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/upload.hex
  COMMAND avr-objcopy -O ihex -R .eeprom ${CMAKE_CURRENT_BINARY_DIR}/solarboot ${CMAKE_CURRENT_BINARY_DIR}/upload.hex 
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/solarboot)

if (avrdudePort)
add_custom_target(upload-solarboot
 ${avrdude} -C${avrdudeConf} -v -patmega328p -cstk500v1 -P${avrdudePort} -b57600 -D -Uflash:w:${CMAKE_CURRENT_BINARY_DIR}/upload.hex:i 
 DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/upload.hex)
else(avrdudePort)
add_custom_target(upload-solarboot
 ${avrdude} -C${avrdudeConf} -v -patmega328p -cstk500v1 -P`ls /dev/ttyUSB*` -b57600 -D -Uflash:w:${CMAKE_CURRENT_BINARY_DIR}/upload.hex:i 
 DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/upload.hex)
endif(avrdudePort)