SOURCE_EXT=c
OBJ_EXT=o
T_EXT=cmocka
TOBJ_EXT=ocmocka
HEAD_EXT=h
OBJ_HEAD_EXT=gch
CC=gcc
CFLAGS=-c -I. -std=c11
LDFLAGS=
DFLAGS=-DDEBUG -ggdb3 -Wall -fdiagnostics-color=auto
RFLAGS=-O2
PFLAGS=-pg
TCFLAGS=-x c
TLFLAGS=-lcmocka
TFILE=$(wildcard *.$(T_EXT))
TOBJECT=$(TFILE:.$(T_EXT)=.$(TOBJ_EXT))
TEXECUTABLE=test.x
MAINOBJ=main.o

COVFLAGS=-fprofile-arcs -ftest-coverage -O0
DEFAULT_DEBUG=y

EXECUTABLE=out.x

SOURCES=$(wildcard *.$(SOURCE_EXT))
OBJECTS=$(SOURCES:.$(SOURCE_EXT)=.$(OBJ_EXT))

# non-file targets
.PHONY: clean cleanall run

ifeq ($(DEFAULT_DEBUG),y)
ALL_TARGET=debug
else
ALL_TARGET=release
endif

all: $(ALL_TARGET)

debug: CFLAGS += $(DFLAGS)
debug: $(SOURCES) $(EXECUTABLE)

win32: CC=i686-w64-mingw32-gcc
win32: $(SOURCES) $(EXECUTABLE)

win64: CC=x86_64-w64-mingw32-gcc
win64: $(SOURCES) $(EXECUTABLE)

profile: CFLAGS +=$(PFLAGS)
profile: release

coverage: CFLAGS +=$(COVFLAGS)
coverage: $(SOURCES) $(EXECUTABLE)

release: CFLAGS += $(RFLAGS)
release: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

%.$(OBJ_EXT): %.$(SOURCE_EXT) $(wildcard *.$(HEAD_EXT))
	$(CC) $(CFLAGS) $< -o $@

build-test: all
	$(CC) $(CFLAGS) $(TCFLAGS) $(TFILE) -o $(TOBJECT)
	$(CC) $(LDFLAGS) $(TLFLAGS) $(TOBJECT) $(filter-out $(MAINOBJ),$(OBJECTS)) -o $(TEXECUTABLE)

test: build-test
	./$(TEXECUTABLE)

cleanall: clean
	rm -f $(EXECUTABLE)

proper: clean cleanall

re: proper all

redo: proper debug

clean:
	rm -f $(wildcard *.$(OBJ_EXT)) $(wildcard *.$(OBJ_HEAD_EXT))

tags:
	ctags -R --c++-kinds=+p --fields=+iaS --extra=+q

run: all
	./$(EXECUTABLE)

gdb: debug
	gdb -q $(EXECUTABLE)

grind: debug
	valgrind ./$(EXECUTABLE)

leak-check: debug
	valgrind --leak-check=full ./$(EXECUTABLE)

