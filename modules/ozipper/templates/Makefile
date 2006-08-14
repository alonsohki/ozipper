include ../Makefile.inc

INSTALL=`which install`

all: build

build:

clean:

install:
	@for i in `ls . --color=no | grep -v CVS | grep -v Makefile | grep -v decoration` ; do \
		echo ${INSTALL} -o ${OWNER} -g ${GROUP} --mode 0440 $$i ${TARGET_DIR}/templates/ ; \
		${INSTALL} -o ${OWNER} -g ${GROUP} --mode 0440 $$i ${TARGET_DIR}/templates/ ; \
	done
	@for i in `ls decoration --color=no | grep -v CVS` ; do \
		echo ${INSTALL} -o ${OWNER} -g ${GROUP} --mode 0440 decoration/$$i ${TARGET_DIR}/templates/decoration/ ; \
		${INSTALL} -o ${OWNER} -g ${GROUP} --mode 0440 decoration/$$i ${TARGET_DIR}/templates/decoration/ ; \
	done
