#!/bin/bash

compiler="gcc -std=c99 -g"

$compiler ../include/duktape/duktape.c -o duktape.o -c
$compiler main.c -o main.o -c

$compiler -lm -o prog duktape.o main.o
