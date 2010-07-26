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
SET(CTUNING "-ffunction-sections -fdata-sections -fno-exceptions -fno-threadsafe-statics")
SET(COPT "-Os -g")
SET(CMCU "-mmcu=atmega328p")
SET(CDEFS "-DF_CPU=16000000L")

SET(CFLAGS "${CMCU} ${CDEBUG} ${CDEFS} ${COPT} ${CWARN} ${CSTANDARD} ${CEXTRA} ${CTUNING}")
SET(CXXFLAGS "${CMCU} ${CDEFS} ${COPT} ${CTUNING} ${CWARN}")
SET(CMAKE_C_FLAGS ${CFLAGS})
SET(CMAKE_CXX_FLAGS ${CXXFLAGS})

add_definitions (-DARDUINO=18)


SET (arduinoDirectory /usr/share/arduino)


include_directories (${arduinoDirectory}/libraries)
include_directories (${arduinoDirectory}/hardware/arduino/cores/arduino)
include_directories (extLib)


add_subdirectory(lib)
add_subdirectory(extLib)
add_subdirectory(Solarboot)
add_subdirectory(Fernbedienung)
add_subdirectory(simple_tests)