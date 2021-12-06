all: gb

gb:
	@ make -f MakeGb.mk all

baixar_gbdk:
	if [ -f /gbdk-linux64.tar.gz ]; then rm -rf gbdk-linux64.tar.gz; fi
	wget https://github.com/gbdk-2020/gbdk-2020/releases/download/4.0.5/gbdk-linux64.tar.gz
	tar -xf gbdk-linux64.tar.gz
	rm gbdk-linux64.tar.gz

rodar_jogo:
	sameboy build/*.gb

clean:
	make -f MakeGb.mk clean