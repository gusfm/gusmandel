#!/bin/bash

make clean
CFLAGS="-g -pg" make

# run program to colect runtime data
./gusmandel

# generate graph.txt
gprof gusmandel gmon.out > graph.txt
