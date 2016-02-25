#!/bin/bash
valgrind --leak-check=full -v ./gusmandel 100 2 ../palletes/pal.ppm
