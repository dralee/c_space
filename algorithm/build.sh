#!/bin/bash
# 2024.11.28 by dralee
# 2025.12.11 by dralee support gdb
file=$1     # the source file
dbg=$2      # the debug flag, eg: -g
path=${file%/*} # file path
full_file=${file##*/} # file.xx
raw_file=${full_file%.*} # raw file name without extension
ext=${full_file##*.} # extension
out_dir=out
res_file=$out_dir/$raw_file.out

CC=gcc
if [ "$ext" = "cpp" ]
then
	CC=g++
fi

if [ -z "$file" ]
then
	echo use the build.sh like \"./build.sh tree/trie.c\" and it will
	echo build for out/trie.out and run it.
	exit 1
else
	echo ok for build args
fi

echo build $file to $res_file
$CC $file -o $res_file $dbg

if [ ! -f "$res_file" ]
then
	echo error for build $res_file
	exit 1
fi

if [ "$dbg" = "-g" ]; then
	echo debug the file $res_file
	gdb $res_file
	exit 0
fi

echo run $res_file
echo ========================================
./$res_file

