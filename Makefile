CC = gcc
BUILD_DIR = ./bin
SRC_DIR = ./src
FILES = $(wildcard ./src/*.c)
OBJS = $(FILES:%=$(BUILD_DIR)/%.o)
DEPS = $(OBJS:.o=.d)
DFLAGS = -g -O0 -Wall -Werror -MMD -MP

# OS-SPECIFIC SETTINGS
ifeq ($(OS),Windows_NT)
  LIBS += -L ./lib
  INCLUDE += -I ./include
  LINK += -l:glfw3.dll -l:libcglm.a
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

$(BUILD_DIR)/EOW: $(OBJS)
	$(CC) $(LIBS) $(OBJS) -o $@ $(LINK)

$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(DFLAGS) $(INCLUDE) -c $< -o $@

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

.PHONY: run
run:
	@./bin/EOW

-include $(DEPS)
