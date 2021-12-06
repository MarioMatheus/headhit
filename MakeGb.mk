NOME_DO_JOGO=foo
ARQUIVO_GB=build/$(NOME_DO_JOGO).gb

DIRETORIO_SOURCES=src
DIRETORIO_OBJ=build

ARQUIVOS_C=$(wildcard $(DIRETORIO_SOURCES)/*.c)
ARQUIVOS_OBJ=$(patsubst $(DIRETORIO_SOURCES)/%.c, $(DIRETORIO_OBJ)/%.o, $(ARQUIVOS_C))

COMPILADOR=gbdk/bin/lcc

FLAGS_DO_COMPILADOR=-Wa-l 				\
					-Wl-m 				\
					-Wf--debug 			\
					-Wl-y 				\
					-Wl-w 				\
					-DUSE_SFR_FOR_REG

all: criar_diretorio_build $(ARQUIVO_GB)

$(ARQUIVO_GB): $(ARQUIVOS_OBJ)
	$(COMPILADOR) $(FLAGS_DO_COMPILADOR) -o $(ARQUIVO_GB) $(ARQUIVOS_OBJ)

$(DIRETORIO_OBJ)/%.o: $(DIRETORIO_SOURCES)/%.c
	$(COMPILADOR) $(FLAGS_DO_COMPILADOR) -c -o $@ $<

criar_diretorio_build:
	@ mkdir -p build

clean:
	@ rm -rf $(DIRETORIO_OBJ)/*