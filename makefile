
#
# Config
#

# Note that if TOOLCHAIN_PREFIX and SDCARD are already defined
# we will not redefine them. 
TOOLCHAIN_PREFIX != arm-linux-gnueabihf-
SDCARD != /Volumes/SDCARD
CC := ${TOOLCHAIN_PREFIX}gcc
CCFLAGS = -static
SRCS = ./src/camera.c ./src/packet.c ./src/serial.c ./src/radio.c


#
# Commands
#

# I use multipass to host a linux VM on my mac.
# It's really annoying having two terminals open,
# so this recipe executes make on that VM
# without having it open a shell to it.
hosted:
	multipass exec primary -d Home/Repos/LadybugWorks/RocketTelemetry make all

# Just compile everything every time
all: bin/telemetry tests

clean:
	rm -Rf bin/*

package:
	cp bin/* ${SDCARD}

#
# Tests
#

tests: bin/uart_test bin/cam_test bin/radio_test

bin/%_test: src/tests/%_test.c FORCE
	${CC} ${CCFLAGS} -o $@ ${SRCS} $<

#
# Target
#

bin/telemetry: FORCE
	${CC} ${CCFLAGS} -o $@ ${SRCS} ./src/main.c



FORCE: ;
