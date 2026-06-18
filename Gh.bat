@echo off 


gcc Graph.c GraphTest.c -o graph


echo Running graph test 

if exist graph.exe (
    graph.exe
) else (
    echo failed
)
exit
