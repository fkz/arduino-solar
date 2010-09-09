cmake_minimum_required(VERSION 2.6)


SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_C_COMPILER avr-gcc)
SET(CMAKE_CXX_COMPILER avr-g++)


PROJECT(arduino)

SET(CMAKE_AR avr-ar)
SET(CMAKE_RANLIB avr-ranlib)


#SET(CSTANDARD "-std=gnu99")
#SET(CDEBUG "-gstabs")
SET(CWARN "-Wall -Wstrict-prototypes")
SET(CTUNING "-ffunction-sections -fdata-sections -fno-exceptions -fno-threadsafe-statics -fpack-struct -fshort-enums")
SET(COPT "-Os -g")
SET(CMCU "-mmcu=atmega328p")
SET(CDEFS "-DF_CPU=16000000L")

SET(CFLAGS "${CMCU} ${CDEBUG} ${CDEFS} ${COPT} ${CWARN} ${CSTANDARD} ${CEXTRA} ${CTUNING}")
SET(CXXFLAGS "${CMCU} ${CDEFS} ${COPT} ${CTUNING} -Wall")
SET(CMAKE_C_FLAGS ${CFLAGS})
SET(CMAKE_CXX_FLAGS ${CXXFLAGS})

add_definitions (-DARDUINO=18)

IF(NOT NO_REALLY_WORLD)
 add_definitions(-DREALLY_WORLD)
 MESSAGE("Compiling for really use")
ELSE(NO_REALLY_WORLD)
 MESSAGE("Compiling for human readable Standard input")
 MESSAGE("this program should not be used together with the XBee")
# MESSAGE("if you want to use that, please add parameter -DREALLY_WORLD=true")
ENDIF(NOT NO_REALLY_WORLD)

if(CMAKE_GENERATOR STREQUAL "MinGW Makefiles")
FILE (TO_NATIVE_PATH C:/Programme/arduino-0018 arduinoDirectory)
SET (avrdude ${arduinoDirectory}/hardware/tools/avr/bin/avrdude)
SET (avrdudeConf ${arduinoDirectory}/hardware/tools/avr/etc/avrdude.conf)
IF (NOT avrdudePort)
SET (avrdudePort com4)
ENDIF (NOT avrdudePort)
else(CMAKE_GENERATOR STREQUAL "MinGW Makefiles")
FILE (TO_NATIVE_PATH /usr/share/arduino arduinoDirectory)
SET (avrdude ${arduinoDirectory}/hardware/tools/avrdude)
SET (avrdudeConf ${arduinoDirectory}/hardware/tools/avrdude.conf)
SET (avrdudePort)
endif(CMAKE_GENERATOR STREQUAL "MinGW Makefiles")

include_directories (${arduinoDirectory}/libraries)
include_directories (${arduinoDirectory}/hardware/arduino/cores/arduino)
include_directories (extLib)
include_directories (${CMAKE_CURRENT_SOURCE_DIR})


add_subdirectory(lib)
add_subdirectory(extLib)
add_subdirectory(Solarboot)
add_subdirectory(Fernbedienung)