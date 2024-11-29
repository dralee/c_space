#!/bin/bash
# 2024.11.28 by dralee
file=$1
path=${file%/*} # file path
full_file=${file##*/} # file.xx
raw_file=${full_file%.*} # raw file name without extension
out_dir=out
res_file=$out_dir/$raw_file.out

if [ -z "$file" ]
then
	echo use the build.sh like \"./build.sh tree/trie.c\" and it will
	echo build for out/trie.out and run it.
	exit 1
else
	echo ok for build args
fi

echo build $file to $res_file
gcc $file -o $res_file

echo run $res_file
echo ========================================
./$res_file
