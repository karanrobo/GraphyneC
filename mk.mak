CC = gcc
CFLAGS = -O3 -Werror

RAYLIB_DIR ?= ../raylib-5.5_win64_mingw-w64

INCLUDES = -I"$(RAYLIB_DIR)/include"
LIBS = -L"$(RAYLIB_DIR)/lib" -lraylib -lopengl32 -lgdi32 -lwinmm
TARGET = graphyne.exe
SRCS = Graph/Graph.c Rendering/Rendering.c Graphyne.c

all: $(TARGET)

$(TARGET): $(SRCS)
    $(CC) $(CFLAGS) $(SRCS) -o $(TARGET) $(INCLUDES) $(LIBS)

clean:
    rm -rf $(TARGET)