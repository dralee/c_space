#!/bin/bash
# 2024.11.28 by dralee
# $1: source file
# $2: option, eg: 17, 20 for the g++ version
file=$1
opt=$2
path=${file%/*} # file path
full_file=${file##*/} # file.xx
raw_file=${full_file%.*} # raw file name without extension
ext=${full_file##*.} # extension
out_dir=out
res_file=$out_dir/$raw_file.out

mkdir -p $out_dir
CC=gcc
if [ "$ext" = "cpp" ]
then
	if [ ! -z "$opt" ];then
		opt="-std=c++$opt"
	fi
	#echo "option: $opt"
	CC="g++ $opt"
	echo now the compiler is $CC
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
#echo "compiler:" $CC
$CC $file -o $res_file

if [ $? -ne 0 ];then
	echo build failed
	exit 1
fi
echo run $res_file
echo ========================================
./$res_file
