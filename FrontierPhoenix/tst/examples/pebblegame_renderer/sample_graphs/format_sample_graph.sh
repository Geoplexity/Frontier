#!/usr/bin/env bash

# Given an input sample graph, format it to include node position information

set -e;

if [[ -z $1 ]]; then
  echo "expected one argument"
  exit 1
fi

input_file="$1"

output_file="${input_file}.out"

neato -Tdot $input_file -o $output_file

mv $output_file $input_file
