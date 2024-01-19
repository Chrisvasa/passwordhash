PROG=programmet.exe
SOURCES=$(wildcard src/*.cpp)
DEPS=$(wildcard include/*.h)
CC=g++
CFLAGS=-Wall -Iinclude -std=c++20
LDFLAGS=-lssl -lcrypto
DEBUG?=1

ifeq ($(DEBUG), 1)
	CFLAGS += -g
	OUTPUTDIR=bin/debug
	PROG=programmet-debug.exe
else
	CFLAGS += -g0 -O3
	OUTPUTDIR=bin/release
endif

OBJS=$(patsubst src/%.cpp,$(OUTPUTDIR)/%.o,$(SOURCES))

$(PROG): $(OUTPUTDIR) $(OBJS)
	$(CC) $(CFLAGS) -o $(PROG) $(OBJS) $(LDFLAGS)

$(OUTPUTDIR)/%.o: src/%.cpp $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@del /q /f "$(OUTPUTDIR)\*"
	@del /q /f $(PROG)

$(OUTPUTDIR):
	@mkdir -p "$(OUTPUTDIR)"

.PHONY: clean