include ../../Makefile.inc
LANG=en
INSTALL=`which install`
FILES=`ls --color=no | grep -v CVS | grep -v LC_MESSAGES | grep -v Makefile`

all: build

build: LC_MESSAGES/ozipper.mo

install: build
	@echo ${INSTALL} --directory -o ${OWNER} -g ${GROUP} --mode 0750 . ${TARGET_DIR}/lang/${LANG}
	@${INSTALL} --directory -o ${OWNER} -g ${GROUP} --mode 0750 . ${TARGET_DIR}/lang/${LANG}
	@echo ${INSTALL} --directory -o ${OWNER} -g ${GROUP} --mode 0750 . ${TARGET_DIR}/lang/${LANG}/LC_MESSAGES
	@${INSTALL} --directory -o ${OWNER} -g ${GROUP} --mode 0750 . ${TARGET_DIR}/lang/${LANG}/LC_MESSAGES
	
	@for i in ${FILES} ; do \
	  echo ${INSTALL} -o ${OWNER} -g ${GROUP} --mode 0440 $$i ${TARGET_DIR}/lang/${LANG}/$$i ; \
		${INSTALL} -o ${OWNER} -g ${GROUP} --mode 0440 $$i ${TARGET_DIR}/lang/${LANG}/$$i ; \
	done
	
	@echo ${INSTALL} -o ${OWNER} -g ${GROUP} --mode 0440 LC_MESSAGES/ozipper.mo ${TARGET_DIR}/lang/${LANG}/LC_MESSAGES/ozipper.mo
	@${INSTALL} -o ${OWNER} -g ${GROUP} --mode 0440 LC_MESSAGES/ozipper.mo ${TARGET_DIR}/lang/${LANG}/LC_MESSAGES/ozipper.mo

clean:
	rm -f LC_MESSAGES/ozipper.mo

LC_MESSAGES/ozipper.mo: LC_MESSAGES/ozipper.po
	msgfmt LC_MESSAGES/ozipper.po -o LC_MESSAGES/ozipper.mo
