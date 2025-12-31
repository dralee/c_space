#!/bin/bash
# build the specific wayland protocol app
# 2025.12.31 by dralee
# $1: main.c file
# $2: output file
# eg: ./build.sh ../hello/main.c wayland-hello
mainfile=$1
outfile=$2

mainfile=$(realpath $mainfile)
dirname=$(dirname $mainfile)

root=$(dirname $(readlink -f $0))

bash $root/xdg-shell-protocol.sh $dirname
cd $dirname

gcc -o $outfile \
    $mainfile \
    $dirname/xdg-shell-protocol.c \
    $(pkg-config --cflags --libs wayland-client wayland-egl egl glesv2)


# run it
$dirname/$outfile