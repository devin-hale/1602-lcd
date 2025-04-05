#!/bin/bash

OCD_DIR="../openocd"

cmake -B ./build && \
cd build && \
make && \

if [[ $1 == "flash" ]]; then
	openocd -f "$OCD_DIR/openocd.cfg"
elif [[ $1 == "debug" ]]; then
	openocd -f "$OCD_DIR/debug.cfg"
fi

cd ..
