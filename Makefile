PROG=programmet.exe
SOURCES=$(wildcard src/*.cpp) # Hämtar alla .cpp filer från src foldern.
DEPS=$(wildcard include/*.h)  # Hämtar alla .h filer från include foldern
OBJS=$(patsubst src/%.cpp,$(OUTPUTDIR)/%.o,$(SOURCES)) # Konverterar .cpp filerna i src till .o filer i bin
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