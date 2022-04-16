.DEFAULT_GOAL := all

.PHONY: cgb
cgb:
	@ make -f MakeGb.mk all

.PHONY: download_gbdk
download_gbdk:
	if [ -f /gbdk-linux64.tar.gz ]; then rm -rf gbdk-linux64.tar.gz; fi
	wget https://github.com/gbdk-2020/gbdk-2020/releases/download/4.0.5/gbdk-linux64.tar.gz
	tar -xf gbdk-linux64.tar.gz
	rm gbdk-linux64.tar.gz

.PHONY: play
play:
	make && sameboy build/*.gb

.PHONY: clean
clean:
	make -f MakeGb.mk clean

.PHONY: all
all: cgb
