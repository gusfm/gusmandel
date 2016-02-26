#!/bin/bash
valgrind --leak-check=full --show-leak-kinds=all -v ./gusmandel 512 100 2 ../palettes/pal.ppm
