CROSS_COMPILER := arm-linux-gnueabihf-
CC := ${CROSS_COMPILER}gcc
CCFLAGS := -static

all: bin/telemetry

bin/telemetry: FORCE
	${CC} ${CCFLAGS} -o $@ src/*.c

tests: bin/test_camera bin/uart_test

bin/test_camera: FORCE
	${CC} ${CCFLAGS} -o $@ src/tests/test_camera.c src/camera.c

bin/uart_test: FORCE
	${CC} ${CCFLAGS} -o $@ src/tests/uart_test.c

export: /Volumes/SDCARD
	rm -Rf /Volumes/SDCARD/*
	cp bin/* /Volumes/SDCARD

FORCE: ;