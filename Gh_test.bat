@echo off

gcc -std=c99 Graph.c GraphTest.c -g -o test


if exist test.exe (
    gdb test.exe
) else (
    echo File not found
)

exit