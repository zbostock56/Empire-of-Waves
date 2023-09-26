FILES = ../../src/island.c ../../src/perlin.c ./src/*.c
LIBS += -L usr/lib/x86_64-linux-gnu/
INCLUDE += -I ../../include -I ./include
LINK += -l:libglfw.so.3.3 -lGL -lX11 -l:libXrandr.so.2 -l:libXi.so.6
LINK += -ldl -lm -l:libcglm.so.0 -lpthread
DFLAGS = -g -O0 -Wall -Werror


.PHONY: default
default:
	gcc $(DFLAGS) $(INCLUDE) $(LIBS) -o test $(FILES) $(LINK)
	@mv test ./bin/

.PHONY: run
run:
	@./bin/test
