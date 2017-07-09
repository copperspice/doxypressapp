ACLOCAL_AMFLAGS= -I autotools/m4 --install
SUBDIRS=. bin

.PHONY: deploy

deploy:
	$(MAKE) -C bin deploy

EXTRA_DIST= \
	README.md \
	CMakeLists.txt \
	src/CMakeLists.txt