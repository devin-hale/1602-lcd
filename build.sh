#!/bin/bash

CMD=$1

OCD_DIR="../openocd"



if [[ "$CMD" == "flash" ]]; then
	cmake -B ./build && \
	cd build && \
	make && \
	openocd
elif [[ "$CMD" == "debug" ]]; then
	cmake -B ./build && \
	cd build && \
	make && \
	openocd -f debug.cfg
elif [[ "$CMD" == "docker" ]]; then

	OPT=$2

	DOCKER=$(command -v docker)

	if [[ "$DOCKER" = "" ]];then
		echo "docker not found"
		exit 1
	fi

	IMAGE_NAME="traffic-light"
	DIMAGE=$(docker images -a | grep "$IMAGE_NAME" | awk '{ print $1}')
	if [[ "$DIMAGE" = "" ]];then
		$DOCKER build -t "$IMAGE_NAME" .
	fi

	$DOCKER run -it --rm -v .:/src $IMAGE_NAME /bin/bash -c "cd /src && ./build.sh"
fi

cd ..
