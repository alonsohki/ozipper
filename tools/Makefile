SUBDIRS=htmlspecialchars

all: build

build:
	@for i in ${SUBDIRS} ; do \
		cd $$i ; \
		make -f Makefile build ; \
		cd .. ; \
	done

clean:
	@for i in ${SUBDIRS} ; do \
		cd $$i ; \
		make -f Makefile clean ; \
		cd .. ; \
	done

install:
	@for i in ${SUBDIRS} ; do \
		cd $$i ; \
		make -f Makefile install ; \
		cd .. ; \
	done

