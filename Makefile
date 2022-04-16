.DEFAULT_GOAL := all

.PHONY: help
help:
	@echo "$$(tput bold)Commands:$$(tput sgr0)"
	@echo
	@sed -n -e "/^## / { \
		h; \
		s/.*//; \
		:doc" \
		-e "H; \
		n; \
		s/^## //; \
		t doc" \
		-e "s/:.*//; \
		G; \
		s/\\n## /---/; \
		s/\\n/ /g; \
		p; \
	}" ${MAKEFILE_LIST} \
	| LC_ALL='C' sort --ignore-case \
	| awk -F '---' \
		-v ncol=$$(tput cols) \
		-v indent=19 \
		-v col_on="$$(tput setaf 6)" \
		-v col_off="$$(tput sgr0)" \
	'{ \
		printf "%s%*s%s ", col_on, -indent, $$1, col_off; \
		n = split($$2, words, " "); \
		line_length = ncol - indent; \
		for (i = 1; i <= n; i++) { \
			line_length -= length(words[i]) + 1; \
			if (line_length <= 0) { \
				line_length = ncol - indent - length(words[i]) - 1; \
				printf "\n%*s ", -indent, " "; \
			} \
			printf "%s ", words[i]; \
		} \
		printf "\n"; \
	}' \
	| more $(shell test $(shell uname) == Darwin && echo '--no-init --raw-control-chars')

.PHONY: cgb
## Compile sources to .gb file
cgb:
	@ make -f MakeGb.mk all

.PHONY: download_gbdk
## Download GBDK_2020 directly from github inside folder
download_gbdk:
	if [ -f /gbdk-linux64.tar.gz ]; then rm -rf gbdk-linux64.tar.gz; fi
	wget https://github.com/gbdk-2020/gbdk-2020/releases/download/4.0.5/gbdk-linux64.tar.gz
	tar -xf gbdk-linux64.tar.gz
	rm gbdk-linux64.tar.gz

.PHONY: play
## Compile .gb file and emulate in emulator sameboy
play:
	make && sameboy build/*.gb

.PHONY: clean
## Call clean in all makefiles
clean:
	make -f MakeGb.mk clean

.PHONY: all
## Call rule cgb
all: cgb
