## CppUTest configurations ##
# Read header comment on file `$(CPPUTEST_HOME)/build/MakefileWorker.mk` to understand these definitions.

COMPONENT_NAME = main
CPPUTEST_HOME = cpputest

CPPUTEST_ENABLE_DEBUG = Y
CPPUTEST_USE_EXTENSIONS = Y
CPP_PLATFORM = Gcc
CPPUTEST_USE_GCOV = Y
CPPUTEST_OBJS_DIR = build/objs/tests
CPPUTEST_LIB_DIR = build/lib/tests

SRC_DIRS = \
    libs

TEST_SRC_DIRS = \
    tests

INCLUDE_DIRS = \
    libs 			\
    $(CPPUTEST_HOME)/include	\

CPPUTEST_CPPFLAGS = -DDISABLE_LOG -DTEST_BUILD
CPPUTEST_CXXFLAGS = -std=c++14 -O0
CPPUTEST_LDFLAGS = -pthread
CPPUTEST_WARNINGFLAGS = -Wall -Wextra -Wshadow -Wswitch-default -Wswitch-enum -Wconversion -Wno-long-long

## The real work ##
# including this file that will use configuration and have the make rules.
include $(CPPUTEST_HOME)/build/MakefileWorker.mk

# Coverage Report rules #
coverage: all
	$(SILENCE)lcov --capture --directory build/objs/tests/libs --output-file coverage.info
	$(SILANCE)genhtml coverage.info --output-directory coverage
	@echo
	@echo "Written coverage report to coverage/index.html"
	@echo

coverage_clean:
	rm -rf coverage coverage.info

