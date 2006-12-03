DIRS=es en de

all: build

build:
	@for i in ${DIRS} ; do \
		make -C $$i $@ ; \
	done

clean:
	@for i in ${DIRS} ; do \
		make -C $$i $@ ; \
	done

install:
	@for i in ${DIRS} ; do \
		make -C $$i $@ ; \
	done

gettext:
	xgettext -k_ --c++ ../src/intl.cpp -o ozipper.po
	@for i in ${DIRS} ; do \
		echo cp ozipper.po $$i/LC_MESSAGES/ozipper.po ; \
		cp ozipper.po $$i/LC_MESSAGES/ozipper.po ; \
	done
	rm -f ozipper.po
