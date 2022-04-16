GAME_NAME=headhit
GB_FILE=build/$(GAME_NAME).gb

SOURCES_DIR=src
OBJ_DIR=build
MODULES := vdata menu lib match
MODULES_DIR := $(patsubst %, $(OBJ_DIR)/%, $(MODULES))

C_FILES=$(shell find $(SOURCES_DIR)/ -type f -name '*.c')
OBJ_FILES=$(patsubst $(SOURCES_DIR)/%.c, $(OBJ_DIR)/%.o, $(C_FILES))

CC=gbdk/bin/lcc

FLAGS=	-Wa-l			\
	-Wl-m 			\
	-Wf--debug 		\
	-Wl-y 			\
	-Wl-w 			\
	-DUSE_SFR_FOR_REG	\

all: create_dir_build $(GB_FILE)

$(GB_FILE): $(OBJ_FILES)
	$(CC) $(FLAGS) -o $(GB_FILE) $(OBJ_FILES)

$(OBJ_DIR)/%.o: $(SOURCES_DIR)/%.c
	$(CC) $(FLAGS) -c -o $@ $<

create_dir_build:
	mkdir -p $(OBJ_DIR) $(MODULES_DIR)

clean:
	rm -rf $(OBJ_DIR)/*
