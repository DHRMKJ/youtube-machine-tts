#!/bin/bash

set -xe

gcc -Wall -Werror -o build/prog ./src/main.c ./src/config.c -I ./include -L ./lib -l curl

#./build/prog

main_dir="./out"
out_dir="./out/build"
v=()

for video in "$main_dir"/videos/*
do
	v+=("$video")
done

ind=0

for audio in "$main_dir"/audios/*
do
 	ffmpeg -i "${v[$ind]}" -i "$audio" -vcodec copy -acodec copy -map 0:0 -map 1:0 "$out_dir/vid$ind.mp4"
	ind=$((ind+1))
	if "$ind">="${#v[@]}"
	then
		break
	fi
done
