# Per cmake docs, 'Generic' used for "Some platforms, e.g. bare metal embedded devices"
set(CMAKE_SYSTEM_NAME Generic)

# Tells CMake to utilize ARM toolchain
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)

# Tells cmake not to auto-link, for executables that require custom flags/linker scripts
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP arm-none-eabi-objdump)
set(CMAKE_SIZE arm-none-eabi-size)
set(CMAKE_DEBUGGER arm-none-eabi-gdb)
set(CMAKE_CPPFILT arm-none-eabi-c++filt)
