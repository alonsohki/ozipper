include ../Makefile.inc

INSTALL=`which install`

all: build

build:

clean:

install:
	@for i in `ls . --color=no | grep -v CVS | grep -v Makefile | grep -v lang` ; do \
		echo ${INSTALL} -o ${OWNER} -g ${GROUP} --mode 0440 $$i ${TARGET_DIR}/public_html/ ; \
		${INSTALL} -o ${OWNER} -g ${GROUP} --mode 0440 $$i ${TARGET_DIR}/public_html/ ; \
	done
	@for i in `ls lang --color=no | grep -v CVS` ; do \
		echo ${INSTALL} -o ${OWNER} -g ${GROUP} --mode 0440 lang/$$i ${TARGET_DIR}/public_html/lang/ ; \
		${INSTALL} -o ${OWNER} -g ${GROUP} --mode 0440 lang/$$i ${TARGET_DIR}/public_html/lang/ ; \
	done
