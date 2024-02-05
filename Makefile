
PROG=programmet.exe
SOURCES=$(wildcard src/*.cpp)
IMGUI_SOURCES=$(wildcard imgui/*.cpp) \
              $(wildcard imgui/backends/imgui_impl_glfw.cpp) \
              $(wildcard imgui/backends/imgui_impl_opengl3.cpp)
DEPS=$(wildcard include/*.h)
CC=g++
CFLAGS=-Wall -std=c++20 -Iimgui -Iimgui/backends -Iinclude -Ipath/to/GLFW/include
LDFLAGS=-lglfw -lGL -ldl -lssl -lcrypto # Updated linker flags for GLFW and OpenGL
DEBUG?=1

ifeq ($(DEBUG), 1)
  CFLAGS += -g
  OUTPUTDIR=bin/debug
  PROG=programmet-debug.exe
else
  CFLAGS += -g0 -O3
  OUTPUTDIR=bin/release
endif

OBJS = $(patsubst src/%.cpp,$(OUTPUTDIR)/%.o,$(SOURCES)) \
  $(addprefix $(OUTPUTDIR)/,$(notdir $(IMGUI_SOURCES:.cpp=.o)))

$(PROG): $(OUTPUTDIR) $(OBJS)
	$(CC) $(CFLAGS) -o $(PROG) $(OBJS) $(LDFLAGS)

$(OUTPUTDIR)/%.o: src/%.cpp $(DEPS)
	$(CC) $(CFLAGS) -o $@ -c $<

# Rule for ImGui source files
$(OUTPUTDIR)/%.o: imgui/%.cpp
	$(CC) $(CFLAGS) -o $@ -c $<

# Rule for ImGui backend source files
$(OUTPUTDIR)/%.o: imgui/backends/%.cpp
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	@rm -rf $(OUTPUTDIR)/*
	@rm -f $(PROG)

$(OUTPUTDIR):
	@mkdir -p $(OUTPUTDIR)

.PHONY: clean test
