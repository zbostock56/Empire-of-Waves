PREVSRC = ../../src/
HELPERS = ../helpers/src/*.c
FILES += $(PREVSRC)globals.c $(PREVSRC)merchant.c $(PREVSRC)player.c $(PREVSRC)island.c
FILES += $(PREVSRC)collision.c  $(PREVSRC)model.c $(PREVSRC)render.c
FILES += $(PREVSRC)controls.c $(PREVSRC)glad.c $(PREVSRC)menu.c $(PREVSRC)perlin.c
FILES += $(PREVSRC)trade_ship.c  $(PREVSRC)enemy.c $(PREVSRC)event.c
FILES += $(PREVSRC)console.c $(PREVSRC)main.c
FILES += $(HELPERS)
FILES += ./src/*.c
LIBS += -L usr/lib/x86_64-linux-gnu/
INCLUDE += -I ../../include -I ./include -I ../helpers/include/
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

.PHONY: clean
clean:
	rm -rf ./bin/*
