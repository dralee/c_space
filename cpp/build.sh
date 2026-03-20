#!/bin/bash
# 2024.11.28 by dralee
# 2026.3.19 by dralee warning
# 2026.3.20 by dralee support build for debug or release version
# $1: source file
# $2: option, debug is the default
#   eg: 17, 20 for the g++ version
#   release for release
#   17-release for release with c++17
file=$1
opt=$2
path=${file%/*} # file path
full_file=${file##*/} # file.xx
raw_file=${full_file%.*} # raw file name without extension
ext=${full_file##*.} # extension
out_dir=out
res_file=$out_dir/$raw_file.out

mkdir -p $out_dir
CC="gcc -Wall -Weffc++ -Wextra -Wconversion -Wsign-conversion -pedantic-errors " # force warnning
if [ "$ext" = "cpp" ]
then
	if [ ! -z "$opt" ];then
		if [[ "$opt" == *"-"* ]];then # 17-release
			items=(${opt//-/ })
			opt1=${items[0]}
			opt2=${items[1]}
			#echo "opt1: $opt1, opt2: $opt2"
			if [ "$opt2" = "release" ];then
				opt="$opt1 -O2 -DNDEBUG" # release
			else
				opt="$opt1 -ggdb" # debug
			fi
			opt="-std=c++$opt"
		elif [ "$opt" = "release" ];then
			opt="-O2 -DNDEBUG" # release
		else
			opt="-std=c++$opt -ggdb" # debug
		fi		
	fi
	#echo the option is \"$opt\"
	
	#echo "option: $opt"
	CC="g++ -Wall -Weffc++ -Wextra -Wconversion -Wsign-conversion -pedantic-errors $opt" # force warnning
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
