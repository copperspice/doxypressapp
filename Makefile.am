ACLOCAL_AMFLAGS= -I autotools/m4 --install
SUBDIRS=. bin

.PHONY: deploy

deploy:
	$(MAKE) -C bin deploy

EXTRA_DIST= \
	CMakeLists.txt \
	cmake/modules/FindCopperSpice.cmake \
	src/CMakeLists.txt