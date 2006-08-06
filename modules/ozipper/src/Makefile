PROGRAM=ozipper.cgi
CFLAGS=-O3 -pipe -Wall -march=i686
LFLAGS=-lpcre
IFLAGS=-I. -I.. -I../include
RM=`which rm`
CC=gcc
CXX=g++
CXXFLAGS=${CFLAGS}


OBJS=ship.o shipfactory.o languagetpl.o xmloutput.o main.o

clean:
	${RM} -f *.o ${PROGRAM}

all: build

build: Makefile ${PROGRAM}

.cpp.o:
	${CXX} ${IFLAGS} ${CXXFLAGS} -c $< -o $@

.c.o:
	${CC} ${IFLAGS} ${CFLAGS} -c $< -o $@

${PROGRAM}: ${OBJS}
	${CXX} ${LFLAGS} ${OBJS} -o $@

ship.o:		ship.cpp		../include/ship.h
shipfactory.o:	shipfactory.cpp		../include/ship.h ../include/shipfactory.h
languagetpl.o:	languagetpl.cpp		../include/languagetpl.h
xmloutput.o:	xmloutput.cpp		../include/xmloutput.h
