include Makefile.inc

SUBDIRS=src lang templates public_html
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
	@echo ${INSTALL} -o ${OWNER} -g ${GROUP} --directory --mode=0755 ${TARGET_DIR}/lang/
	@${INSTALL} -o ${OWNER} -g ${GROUP} --directory --mode=0755 ${TARGET_DIR}/lang/
	@echo ${INSTALL} -o ${OWNER} -g ${GROUP} --directory --mode=0755 ${TARGET_DIR}/templates/decoration/
	@${INSTALL} -o ${OWNER} -g ${GROUP} --directory --mode=0755 ${TARGET_DIR}/templates/decoration/
	@echo ${INSTALL} -o ${OWNER} -g ${GROUP} --directory --mode=0755 ${TARGET_DIR}/public_html/cgi-bin/
	@${INSTALL} -o ${OWNER} -g ${GROUP} --directory --mode=0755 ${TARGET_DIR}/public_html/cgi-bin/
	@echo ${INSTALL} -o ${OWNER} -g ${GROUP} --directory --mode=0755 ${TARGET_DIR}/public_html/lang/
	@${INSTALL} -o ${OWNER} -g ${GROUP} --directory --mode=0755 ${TARGET_DIR}/public_html/lang/
	@for i in ${SUBDIRS} ; do \
		make -C $$i install ; \
	done
