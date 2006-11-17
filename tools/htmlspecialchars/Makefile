include ../../Makefile.inc

PROGRAM=htmlspecialchars
CC=gcc
CFLAGS=-O3 -pipe -Wall
LFLAGS=
WGET=/usr/bin/wget
RM=/usr/bin/rm
ENTURL=http://www.w3.org/TR/html4/HTMLlat1.ent

all: build

build: ${PROGRAM}

.c.o:
	${CC} ${CFLAGS} -c $< -o $@

OBJS=main.o

${PROGRAM}: entities-latin1.dtd ${OBJS}
	${CC} ${LFLAGS} ${OBJS} -o $@

clean:
	${RM} -f *.o entities-latin1.dtd

main.o:	main.c

entities-latin1.dtd:
	wget -c ${ENTURL} -O $@
