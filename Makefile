# Uncomment lines below if you have problems with $PATH
#SHELL := /bin/bash
#PATH := /usr/local/bin:$(PATH)
HOME_DIR ?= $(shell echo ~)

all: compile copy_bins

env: compile-env copy_bins

compile:
	platformio -f  run
	
compile-env:
	platformio -f  run -e $(build)

upload:
	platformio -f  run -e $(build) --target upload

clean:
	platformio -f  run --target clean
	rm -f *.bin

program:
	platformio -f  run --target program

uploadfs:
	platformio -f  run --target uploadfs

update:
	platformio -f  update
	
install-ota:
	${HOME_DIR}/tools/espota.py -f ${build}.bin -r -i $(host) -p 3232 -P 32323
	
copy_bins:
	@for f in .pio/build/*/firmware.bin; do \
		cp "$$f" "./$$(basename $$(dirname "$$f")).bin"; \
	done
