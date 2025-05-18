FROM debian:12.10
RUN apt update -y
RUN apt upgrade -y
RUN apt install gcc-arm-none-eabi gdb-arm-none-eabi binutils-arm-none-eabi libnewlib-arm-none-eabi cmake make openocd -y
