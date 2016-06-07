#!/bin/bash
gcc -std=c99 -D_XOPEN_SOURCE pr1.c -o pr1
gcc -std=c99 -D_XOPEN_SOURCE pr2.c -o pr2
gcc -std=c99 -D_XOPEN_SOURCE pr1_geom.c -o pr1_geom