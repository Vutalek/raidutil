TARGET = raid
PREFIX = /usr/local/bin
LIB = string.o cmdoutput.o regex.o cleaner.o
COMMANDS = status.o get.o replace.o
BLIB = build/string.o build/cmdoutput.o build/regex.o build/cleaner.o
BCOMMANDS = build/status.o build/get.o build/replace.o

.PHONY: all clean install uninstall

all: $(TARGET)

clean:
	rm -rf bin/$(TARGET) build/*.o
	rmdir bin
	rmdir build

install: $(TARGET)
	install bin/$(TARGET) $(PREFIX)

uninstall:
	rm -rf $(PREFIX)/$(TARGET)

build:
	mkdir build

bin:
	mkdir bin

$(TARGET): bin $(LIB) $(COMMANDS) raidutil.o
	gcc -Wall -o bin/$(TARGET) $(BLIB) $(BCOMMANDS) build/raidutil.o

string.o: build
	gcc -c -o build/string.o commands/lib/string.c

cmdoutput.o: build
	gcc -c -o build/cmdoutput.o commands/lib/cmdoutput.c

regex.o: build
	gcc -c -o build/regex.o commands/lib/regex.c

cleaner.o: build
	gcc -c -o build/cleaner.o commands/lib/cleaner.c

status.o: build
	gcc -c -o build/status.o commands/status.c

get.o: build
	gcc -c -o build/get.o commands/get.c

replace.o: build
	gcc -c -o build/replace.o commands/replace.c

raidutil.o: build
	gcc -c -o build/raidutil.o raidutil.c
