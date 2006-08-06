include Makefile.inc

SUBDIRS=src lang
INSTALL=`which install`

all: build

build:
	@for i in ${SUBDIRS} ; do \
		make -C $$i build ; \
	done

clean:
	@for i in ${SUBDIRS} ; do \
		make -C $$i clean ; \
	done

install:
	@echo ${INSTALL} --directory --mode=0755 ${TARGET_DIR}/lang/
	@${INSTALL} --directory --mode=0755 ${TARGET_DIR}/lang/
	@for i in ${SUBDIRS} ; do \
		make -C $$i install ; \
	done
