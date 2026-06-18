@echo off 


gcc Graph.c Graphyne.c -o graph -I"C:\Users\karan\Desktop\CompUni\Major Projects\raylib-5.5_win64_mingw-w64\include" -L"C:\Users\karan\Desktop\CompUni\Major Projects\raylib-5.5_win64_mingw-w64\lib" -lraylib -lopengl32 -lgdi32 -lwinmm


echo Running graph test 

if exist graph.exe (
    graph.exe
) else (
    echo failed
)
exit
