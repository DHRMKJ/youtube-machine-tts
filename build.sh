#!/bin/sh

set -xe

gcc -Wall -Werror -o build/prog ./src/main.c ./src/config.c -I ./include -L ./lib -l curl

./build/prog
 

#indir = "/output/"
#outdir = "/output/build"

#mkdir -p "$outdir"

#videos=$(ls -1 "$indir/videos"/*.mp4 | sort)
#audios=$(ls -1 "$indir/audios"/*.mp3 |sort)

#if [ ${#videos[@]} -eq ${#audios[@]} ]; then
#	for ((i=0; i<${vidoes[@]}; i++)); do
#		video_file="${videos[$i]}"
#		audio_file="${audios[$i]}"
#
#		vname=$(basename "${video_file%.*}")
#		output_vid="$outdir/vid$i.mp4"

#		ffmpeg -i "$video_file" -i "$audios[$i]" -vcodec copy -acodec copy -map 0:0 -map 1:0 "$output_vid"
#		echo "Processed: $output_vid"
#	done
#else 
#	echo "Error: shit happened"
#fi

