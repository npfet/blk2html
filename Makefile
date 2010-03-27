blk2html: blk2html.o
	gcc -o blk2html blk2html.o
	
blk2html.o: blk2html.c
	gcc -c blk2html.c

blkhtml: blk2html color.blk loop.sh
	echo "<BODY>" >output/blk.html
	./range 18 63 ./loop.sh 
	echo "</BODY>" >>output/blk.html

clean:
	rm -f blk2html.o blk2html


