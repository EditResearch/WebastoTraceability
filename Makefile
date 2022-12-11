CC=gcc
CFLAGS=-Wall -Wextra -pedantic -std=c18 -O2 $$(pkg-config --cflags gtk+-3.0 sqlite3) 
LIBS=$$(pkg-config --libs gtk+-3.0 sqlite3) -lendian -llog

TARGET=traceability
CACHE=.cache
OUTPUT=$(CACHE)/release

MODULES += main.o
MODULES += view.o
MODULES += ui.o
MODULES += controller.o
MODULES += model.o

TEST += test.o


OBJ=$(addprefix $(CACHE)/,$(MODULES))

T_OBJ=$(addprefix $(CACHE),$(TEST))

all: env $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBS) -o $(OUTPUT)/$(TARGET)


%.o:
	$(CC) $(CFLAGS) -c $< -o $@


-include dep.list


exec: all
	$(OUTPUT)/$(TARGET)


.PHONY: env dep clean


dep:
	$(CC) -MM  app/*.c src/*.c | sed 's|[a-zA-Z0-9_-]*\.o|$(CACHE)/&|' > dep.list


env:
	mkdir -pv $(CACHE)
	mkdir -pv $(OUTPUT)


clean: 
	rm -rvf $(OUTPUT)
	rm -vf $(CACHE)/*.o



