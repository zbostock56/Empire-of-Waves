CC = gcc
BUILD_DIR := ./bin
SRC_DIR := ./src
FILES = $(wildcard ./src/*.c)
OBJS = $(FILES:%=$(BUILD_DIR)/%.o)
DEPS = $(OBJS:.o=.d)
DFLAGS = -g -O3 -Wall -Werror -MMD -MP
FLAG = 0

# OS-SPECIFIC SETTINGS
ifeq ($(OS),Windows_NT)
  LIBS += -L ./lib
  INCLUDE += -I ./include
  LINK += -l:glfw3.dll -l:libcglm.a -lopengl32
else
  detected_OS := $(shell uname)
  ifeq ($(detected_OS),Linux)
    # LINUX-DEPENDENT SETTINGS
    LIBS += -L usr/lib/x86_64-linux-gnu/
    INCLUDE += -I ./include
    LINK += -l:libglfw.so.3.3 -lGL -lX11 -l:libXrandr.so.2 -l:libXi.so.6
		LINK += -ldl -lm -l:libcglm.so.0 -lpthread
  endif
endif

.PHONY: clean run debug

all: ./bin/src $(BUILD_DIR)/EOW

$(BUILD_DIR)/EOW: $(OBJS)
	$(CC) $(LIBS) $(OBJS) -o $@ $(LINK)

$(BUILD_DIR)/%.c.o: %.c
	$(CC) $(DFLAGS) $(INCLUDE) -c $< -o $@

./bin/src:
	mkdir -p ./bin/src

clean:
	rm -rf $(BUILD_DIR)

run:
	@./bin/EOW

debug:
	gdb ./bin/EOW

-include $(DEPS)
