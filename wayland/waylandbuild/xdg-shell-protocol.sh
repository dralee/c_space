#!/bin/bash
# xdg shell using wayland-scanner generate protocol
# 2025.12.31 by dralee
# $1: target path

target=$1

# 协议XML文件位置
PROTO_DIR=/usr/share/wayland-protocols/stable/xdg-shell

# 生成协议头文件
wayland-scanner client-header $PROTO_DIR/xdg-shell.xml xdg-shell-client-protocol.h

# 生成协议源文件
wayland-scanner private-code $PROTO_DIR/xdg-shell.xml xdg-shell-protocol.c

if [[ -d $target ]]; then
    mv xdg-shell-client-protocol.h $target
    mv xdg-shell-protocol.c $target
fi