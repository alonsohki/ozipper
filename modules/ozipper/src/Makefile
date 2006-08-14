include ../Makefile.inc

PROGRAM=ozipper.cgi
CFLAGS=-O3 -pipe -Wall -march=i686
LFLAGS=-lpcre
IFLAGS=-I. -I.. -I../include
RM=`which rm`
INSTALL=`which install`
CC=gcc
CXX=g++
CXXFLAGS=${CFLAGS}

all: build

OBJS=ship.o shipfactory.o player.o languagetpl.o http.o main.o

clean:
	${RM} -f *.o

install: build
	@echo ${INSTALL} --mode=0750 --strip ${PROGRAM} ${TARGET_DIR}/
	@${INSTALL} --mode=0750 --strip ${PROGRAM} ${TARGET_DIR}/

build: Makefile ${PROGRAM}

.cpp.o:
	${CXX} ${IFLAGS} ${CXXFLAGS} -c $< -o $@

.c.o:
	${CC} ${IFLAGS} ${CFLAGS} -c $< -o $@

${PROGRAM}: ${OBJS}
	${CXX} ${LFLAGS} ${OBJS} -o $@

ship.o:		ship.cpp		../include/ship.h
shipfactory.o:	shipfactory.cpp		../include/ship.h ../include/shipfactory.h
player.o:	player.cpp		../include/player.h ../include/exception.h
languagetpl.o:	languagetpl.cpp		../include/languagetpl.h ../include/shipfactory.h ../include/exception.h ../include/player.h
http.o:		http.cpp		../include/http.h
main.o:		main.cpp		../include/languagetpl.h ../include/exception.h ../include/http.h ../include/shipfactory.h
