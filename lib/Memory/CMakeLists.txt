add_library (MemoryManagement STATIC MemoryManagement.cpp serialize.cpp)
target_link_libraries(MemoryManagement EEPROM)
