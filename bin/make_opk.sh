#!/bin/sh

rm -f fba-rg350.opk

mksquashfs skin fbasdl.dge sdl_font.ttf skin/fba_icon.png default.gcw0.desktop ../readme.txt fba-rg350.opk -all-root -no-xattrs -noappend -no-exports
