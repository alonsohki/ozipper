include ../Makefile.inc

PROGRAM=ozipper.cgi
CFLAGS=-O3 -pipe -Wall -march=i686 `xml2-config --cflags` `xslt-config --cflags`
LFLAGS=-lpcre `xml2-config --libs` `xslt-config --libs`
IFLAGS=-I. -I.. -I../include
RM=`which rm`
INSTALL=`which install`
CC=gcc
CXX=g++
CXXFLAGS=${CFLAGS}

all: build

OBJS=ship.o shipfactory.o player.o languagetpl.o http.o main.o

clean:
	@echo ${RM} -f *.o
	@${RM} -f *.o

install: build
	@echo ${INSTALL} -o ${OWNER} -g ${GROUP} --mode=0750 --strip ${PROGRAM} ${TARGET_DIR}/public_html/cgi-bin/
	@${INSTALL} -o ${OWNER} -g ${GROUP} --mode=0750 --strip ${PROGRAM} ${TARGET_DIR}/public_html/cgi-bin/

build: Makefile ${PROGRAM}

.cpp.o:
	@echo ${CXX} ${IFLAGS} ${CXXFLAGS} -c $< -o $@
	@${CXX} ${IFLAGS} ${CXXFLAGS} -c $< -o $@

.c.o:
	@echo ${CC} ${IFLAGS} ${CFLAGS} -c $< -o $@
	@${CC} ${IFLAGS} ${CFLAGS} -c $< -o $@

${PROGRAM}: ${OBJS}
	@echo ${CXX} ${LFLAGS} ${OBJS} -o $@
	@${CXX} ${LFLAGS} ${OBJS} -o $@

ship.o:		ship.cpp		../include/ship.h
shipfactory.o:	shipfactory.cpp		../include/ship.h ../include/shipfactory.h
player.o:	player.cpp		../include/player.h ../include/exception.h
languagetpl.o:	languagetpl.cpp		../include/languagetpl.h ../include/shipfactory.h ../include/exception.h ../include/player.h
http.o:		http.cpp		../include/http.h
main.o:		main.cpp		../include/languagetpl.h ../include/exception.h ../include/http.h ../include/shipfactory.h
