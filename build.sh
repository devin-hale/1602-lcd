#!/bin/bash

OCD_DIR="../openocd"

cmake -B ./build && \
cd build && \
make && \

if [[ $1 == "flash" ]]; then
	openocd
elif [[ $1 == "debug" ]]; then
	openocd -f debug.cfg
fi

cd ..
