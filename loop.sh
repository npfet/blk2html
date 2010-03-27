#!/bin/sh -- 

BLKFILE="color.blk"

echo Outputing $1...
echo "Block $1<BR><BR>" >>output/blk.html
./blk2html $BLKFILE $1 >>output/blk.html
echo "<HR>">>output/blk.html
