cmake_minimum_required(VERSION 3.31)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(lcd-1602)

set(ELF main.elf)

enable_language(ASM)
set(STARTUP_FILE "startup.c")

add_executable(${ELF}
	${STARTUP_FILE}
	src/main.c
)

set_target_properties(
  ${ELF}
  PROPERTIES
  LINK_FLAGS
  "-T${PROJECT_SOURCE_DIR}/stm32f446.ld \
   -mthumb -mcpu=cortex-m4 \
   -Wl,--no-warn-rwx-segment \
   -Wl,--gc-sections \
   -Wl,-Map=${PROJECT_NAME}.map")

target_compile_options(${ELF} PUBLIC
  -Wall 
  -g 
  -std=gnu99 
  -Os
  -mthumb
  -mcpu=cortex-m4
  -mfloat-abi=soft
  -mlittle-endian
  -ffunction-sections 
  -fdata-sections
  -Werror 
  -Wstrict-prototypes 
  -Warray-bounds 
  -fno-strict-aliasing 
  -Wno-unused-const-variable 
  -specs=nano.specs 
  -specs=nosys.specs)

file(COPY
  openocd.cfg
  DESTINATION
  ${CMAKE_BINARY_DIR})
