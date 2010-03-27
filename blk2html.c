/* Copyright (c) 2001 Nathaniel Pinckney */


#include <stdio.h>
#include <stdlib.h>

long getword(void);

FILE *STREAM;
int count;

char *last;
char chars[] = " rtoeanismcylgfwdvpbhxuq0123456789j-k.z/;:!+@*,?";

char *unpack(long word) {
	static char buf[8];
	int i;
	for(i=0;i<7;i++) {
		if(word & 0x80000000) {
			word = word << 1;
			if(word & 0x80000000) {
				word = word << 1;
				buf[i] = chars[((word >> 27) & 0x1F)+16];
				word = word << 5;
			} else {
				buf[i] = chars[((word >> 28) & 7)+8];
				word = word << 4;
			}
		} else {
			buf[i] = chars[(word >> 28) & 7];
			word = word << 4;
		}
	}
	for(i=6;i>=0;i--) {
		if(buf[i] == ' ')
			buf[i] = 0;
		else
			break;
	}
	buf[7] = 0;
	return buf;
}
	

void nul(long word) {
}

void exten(long word) {
	if(word & 0xFFFFFFF0) {
		printf("<FONT COLOR=\"%s\">%s</FONT>",last,unpack(word  & 0xFFFFFFF0));
	} else {
		// end interpretation
	}
}
void execute(long word) {
	last="yellow";
	printf("<FONT COLOR=\"yellow\">%s</FONT>",unpack(word & 0xFFFFFFF0));
}
void comment(long word) {
	last="black";
	printf("<FONT COLOR=\"black\">%s</FONT>",unpack(word  & 0xFFFFFFF0));
}
void red(long word) {
	last="red";
	printf("<BR><FONT COLOR=\"red\">%s</FONT>",unpack(word & 0xFFFFFFF0));
}
void NUM(long word) {
	last="yellow";
	if((word >> 4) & 1)
		printf("<FONT COLOR=\"yellow\">0x%X</FONT>",getword());
	else
		printf("<FONT COLOR=\"yellow\">%i</FONT>",getword());
}
void cNUM(long word) {
	last="green";
	if((word >> 4) & 1)
		printf("<FONT COLOR=\"green\">0x%X</FONT>",getword());
	else	
		printf("<FONT COLOR=\"green\">%i</FONT>",getword());
}
void SHORT(long word) {
	last="yellow";
	if((word >> 4) & 1)
		printf("<FONT COLOR=\"yellow\">0x%X</FONT>",word >> 5);
	else
		printf("<FONT COLOR=\"yellow\">%i</FONT>",word >> 5);
}
void cSHORT(long word) {
	last="green";
	if((word >> 4) & 1)
		printf("<FONT COLOR=\"green\">0x%X</FONT>",word >> 5);
	else
		printf("<FONT COLOR=\"green\">%i</FONT>",word >> 5);
}

void qCOMPILE(long word) {
	last="green";
	printf("<FONT COLOR=\"green\">%s</FONT>",unpack(word & 0xFFFFFFF0));
}
void COMPILE(long word) {
	last="cyan";
	printf("<FONT COLOR=\"cyan\">%s</FONT>",unpack(word & 0xFFFFFFF0));
}

void variable(long word) {
	last="magenta";
	printf("<FONT COLOR=\"magenta\">%s %u</FONT>",unpack(word & 0xFFFFFFF0),getword());
}


void (*typetbl[])(long) = {
	exten, execute, NUM, red,
	qCOMPILE, cNUM, cSHORT, COMPILE,
	SHORT, comment, nul, nul,
	variable, nul, nul, nul,
};


void doit(long int word) {
	typetbl[word & 0x0F](word);
}

long getword(void) {
	long int word;
	word = fgetc(STREAM) & 0xFF;
	word += (fgetc(STREAM) & 0xFF) <<8;
	word += (fgetc(STREAM) & 0xFF) <<16;
	word += (fgetc(STREAM) & 0xFF) <<24;
	count++;
	return word;
}

void gotoblock(int block) {
	fseek(STREAM,-12*1024+block*1024,SEEK_SET);
}

void dumpblock(void) {
	long word;
	for(count = 0; count < 256;) { // actually should end when we hit
				   // 00000000h extension word
		word = getword();
		if((word & 0x0F) != 0) {
			printf("&nbsp;&nbsp;");
		}
		doit(word);
	}
}

int main(int argc, char *argv[]) {
	if(argc != 3) {
		fprintf(stderr,"Usage: blk2html <.blk file> <block number>\n");
		exit(1);
	}
	STREAM = fopen(argv[1],"r");
	if(!STREAM) {
		fprintf(stderr,"Couldn't open .blk file \"%s\"!\n",argv[1]);
		exit(1);
	}
	gotoblock(atoi(argv[2]));
	last = "black";
	dumpblock();
	fclose(STREAM);
	return 0;
}
