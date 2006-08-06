include ../Makefile.inc

DIRS=`ls . --color=no | grep -v CVS | grep -v Makefile`
INSTALL=`which install`

all: build

build:

clean:

install:
	@for i in ${DIRS} ; do \
		echo ${INSTALL} --directory --mode 0750 $$i ${TARGET_DIR}/lang/$$i ; \
		${INSTALL} --directory --mode 0750 $$i ${TARGET_DIR}/lang/$$i ; \
	done
	@for i in ${DIRS} ; do \
		for x in `ls $$i --color=no | grep -v CVS` ; do \
			echo ${INSTALL} --mode 0440 $$i/$$x ${TARGET_DIR}/lang/$$i/ ; \
			${INSTALL} --mode 0440 $$i/$$x ${TARGET_DIR}/lang/$$i/ ; \
		done ; \
	done
