#!/bin/sh

set -xe

gcc -Wall -Werror -o build/prog ./src/main.c ./src/config.c -I ./include -L ./lib -l curl

./build/prog
