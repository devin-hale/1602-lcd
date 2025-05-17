cmake_minimum_required(VERSION 4.0.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/toolchain.cmake)

project(lcd-1602 VERSION 0.0.1)

set(ELF firmware.elf)

enable_language(ASM)

file(GLOB_RECURSE sources src/*.c)
add_executable(${ELF}
	${sources}
	${PROJECT_SOURCE_DIR}/vendor/cmsis-device-f4/Source/Templates/gcc/startup_stm32f446xx.s
)

target_include_directories(${ELF}
	PUBLIC
	${PROJECT_SOURCE_DIR}/inc
	${PROJECT_SOURCE_DIR}/vendor/CMSIS_5/CMSIS/Core/Include
	${PROJECT_SOURCE_DIR}/vendor/cmsis-device-f4/Include
)

target_link_options(${ELF} PUBLIC
	-T${PROJECT_SOURCE_DIR}/linker.ld # specify linker script
	-nostartfiles
	--specs=nano.specs
	--specs=nosys.specs
	-nostdlib
	-mcpu=cortex-m4
	-mthumb
	-Wl,--no-warn-rwx-segment
	-Wl,--gc-sections # prune unused code, requires -ffunction-sections and -fdata-sections flag in compiler
	-Wl,-Map=${PROJECT_NAME}.map # creates section mapping for debugging
)

target_compile_options(${ELF} PUBLIC
	# -- General Options --
  	-g3 # debug information level 3
	-Os # optimize for size
	# place each function or data item into its own section
	# allows for optimization in linkage
	-ffunction-sections
  	-fdata-sections
	-I/usr/arm-none-eabi/include

	# -- ARM Options --
  	-mthumb # compressed 16bit thumb state
  	-mcpu=cortex-m4 # specify architecture
	# floating point handling. stm32f4xx supports single precision.
	# 'soft' for software, 'hard' for hardware
  	-mfloat-abi=soft 
  	-mlittle-endian  # obvious, set to little endian
	# -- Warning/Error Options ---
	-Wall
	-Wextra
  	-Werror # warnings returned as errors
	-Wundef
	-Wshadow
	-Wdouble-promotion
	-Wformat-truncation
	-Wconversion
  	-fno-strict-aliasing 
  	-Wno-unused-const-variable 
)

file(COPY ${CMAKE_SOURCE_DIR}/openocd/openocd.cfg DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/openocd/debug.cfg DESTINATION ${CMAKE_BINARY_DIR})
