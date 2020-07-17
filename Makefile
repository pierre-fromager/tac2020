CXX = gcc
CC = $(CXX)
CFLAGS = -Wall -Wextra -std=c11 -lpthread -lm -g
SRC = $(wildcard src/*.c) $(wildcard src/**/*.c)
OBJECTS=$(SRC:%.c=%.o)
TARGET=tac

.PHONY: all
	all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(LINK.c) $^ $(LOADLIBES) $(LDLIBS) -o $@

.PHONY: clean
clean:
	rm -rf $(TARGET) $(OBJECTS)

.PHONY: trace
trace:
	valgrind --tool=drd --show-stack-usage=yes ./$(TARGET)

.PHONY: debug
debug:
	gdb ./$(TARGET)

.PHONY: doc
doc:
	doxygen doc/tac-dox.conf

.PHONY: cleandoc
cleandoc:
	rm -rf doc/html