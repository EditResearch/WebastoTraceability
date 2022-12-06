CC=gcc
CFLAGS=-Wall -Wextra -pedantic -std=c18 -O2 $$(pkg-config --cflags gtk+-3.0)
LIBS=$$(pkg-config --libs gtk+-3.0)

TARGET=traceability
CACHE=.cache
OUTPUT=$(CACHE)/release

MODULES += main.o


OBJ=$(addprefix $(CACHE)/,$(MODULES))


all: env $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBS) -o $(OUTPUT)/$(TARGET)


%.o:
	$(CC) $(CFLAGS) -c $< -o $@


-include dep.list


exec: all
	$(OUTPUT)/$(TARGET)


.PHONY: env dep clean


dep:
	$(CC) -MM app/*.c | sed 's|[a-zA-Z0-9_-]*\.o|$(CACHE)/&|' > dep.list


env:
	mkdir -pv $(CACHE)
	mkdir -pv $(OUTPUT)


clean: 
	rm -rvf $(OUTPUT)
	rm -vf $(CACHE)/*.o



