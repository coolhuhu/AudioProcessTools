#!/bin/bash

# Usage: <input_directory> <output_directory> <input_suffix> <output_suffix> <ffmpeg_command>

if [ "$#" -ne 5 ]; then
  echo "Usage: <input_directory> <output_directory> <input_suffix> <output_suffix> <ffmpeg_command>"
  echo "Usage such as: ./BatchFFmpeg.sh input_dir output_dir .pcm .wav ffmpeg_command"
  echo "ffmpeg_command such as: 'ffmpeg -f s16le -ar 8000 -ac 1 -i \$input -ar 16000 -f wav \$output'"
fi

if [ ! -d $1 ]; then
  echo "Error: input directory does not exit."
  exit 1
fi

if [ ! -d $2 ]; then
  echo "output directory $2 does not exit. create it."
  mkdir -p $2
fi

input_abspath=$(realpath $1)
output_abspath=$(realpath $2)
command="${!#}"


cd ../ffmpeg/bin

start_time=$(date +%s.%3N)

find "$input_abspath" -type f -iname "*.pcm" | while read file; do
  abspath=$(realpath $file)
  input="$(realpath $file)"
  basefilename=$(basename $file)
  basefilename_noext=${basefilename%.*}
  output="$output_abspath/$basefilename_noext.wav"

  #./ffmpeg -f s16le -ar 8000 -ac 1 -i $input -ar 16000 -f wav $output
  eval $command
done

end_time=$(date +%s.%3N)

cd -

echo "convert done!"

elapsed_time=$(echo "$end_time - $start_time" | bc)

printf "Execution time: %.3f seconds\n" $elapsed_time

# TODO add multi threads