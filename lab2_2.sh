#!/bin/bash


if [[ $# -lt 1 ]]; then
  echo "Error: Missing directory path argument." >&2
  exit 1
fi

dir_path="$1"
search_string="${2:-error}" 


if [[ ! -d "$dir_path" ]]; then
  echo "Error: $dir_path is not a directory or does not exist." >&2
  exit 1
fi


result=$(grep -rl "$search_string" "$dir_path")


if [[ -z "$result" ]]; then
  echo "No file in $dir_path or its subdirectories contains '$search_string'."
  exit 0
fi


echo "$result"
 
