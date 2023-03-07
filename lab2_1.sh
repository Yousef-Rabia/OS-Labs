#!/bin/bash


if [[ $# -eq 0 ]]; then
  dir_path="."
else

  dir_path="$1"
fi


if [[ ! -d "$dir_path" ]]; then
  echo "Error: $dir_path is not a directory or does not exist." 
  exit 1
fi


find "$dir_path" -maxdepth 1 -type f -name "*.txt"

