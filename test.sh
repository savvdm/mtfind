#!/usr/bin/env bash

pattern="${1:?Specify search string}"
file=${2:?Specify file name for command output}

diff <(./mtfind input.txt "$pattern") $file 
