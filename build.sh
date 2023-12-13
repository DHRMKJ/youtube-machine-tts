#!/bin/sh

set -xe

gcc -Wall -Werror -o prog ./src/main.c ./src/config.c -I ./include -L ./lib -l curl

./prog
