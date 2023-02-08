CROSS_COMPILER := arm-linux-gnueabihf-
CC := ${CROSS_COMPILER}gcc
CCFLAGS := -static

all: bin/telemetry

bin/telemetry: FORCE
	${CC} ${CCFLAGS} -o $@ src/*.c

tests: bin/uart_test bin/cam_test

bin/uart_test: FORCE
	${CC} ${CCFLAGS} -o $@ src/tests/uart_test.c src/serial.c

bin/cam_test: FORCE
	${CC} ${CCFLAGS} -o $@ src/tests/cam_test.c src/camera.c src/serial.c

export: /Volumes/SDCARD
	rm -Rf /Volumes/SDCARD/*
	cp bin/* /Volumes/SDCARD

FORCE: ;