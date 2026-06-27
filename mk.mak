CC = gcc
CFLAGS = -O3 -Werror
INCLUDES = -I"C:\Users\karan\Desktop\CompUni\Major Projects\raylib-5.5_win64_mingw-w64\include" 
LIBS = -L"C:\Users\karan\Desktop\CompUni\Major Projects\raylib-5.5_win64_mingw-w64\lib" -lraylib -lopengl32 -lgdi32 -lwinmm
TARGET = graphyne.exe
SRCS = Graph/Graph.c Rendering/Rendering.c Graphyne.c


all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET) $(INCLUDES) $(LIBS)


clean:
	rm -rf $(TARGET)