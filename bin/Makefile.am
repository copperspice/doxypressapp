include copperspice.am

# dist
distdir_custom=source_dist/dist/DoxyPressApp-$(VERSION)

# path to gcc retrieved from configure (windows)
gccpath=$(dir $(shell which $(word 1,$(CC))))

# list of gcc libraries (windows)  sjlj  or seh
gcclibs=libgcc_s_*-1  libstdc++-6  libwinpthread-1

# list of CopperSpice libraries
cslibs=Core Gui Network Xml

# location of includes
AM_CXXFLAGS+=-I../src

if OSTYPE_LINUX
endif

if OSTYPE_DARWIN
endif

if OSTYPE_WIN
   # disable console on window
   AM_CXXFLAGS+=-mwindows

   # location for 'make install'
   installdir=$(abs_top_builddir)/install
endif

# target app name
bin_PROGRAMS = DoxyPressApp

# copy required CopperSpice and gcc files for deployment
.PHONY: deploy

if OSTYPE_LINUX
endif

if OSTYPE_DARWIN
deployapp=$(deploydir)/$(bin_PROGRAMS).app/Contents

deploy : all
	mkdir -p $(deployapp)/Frameworks
	mkdir -p $(deployapp)/MacOS
	mkdir -p $(deployapp)/Resources

	$(LIBTOOL) --mode=install cp $(bin_PROGRAMS) $(deployapp)/MacOS

   # copy doxypress app into this dmg
	cp $(top_srcdir)/../DoxyPress/bin/doxypress  $(deployapp)/MacOS

	rm -f $(deployapp)/Info.plist
	sed -e "s,@ICON@,doxypress.icns,g" \
	    -e "s,@TYPEINFO@,????,g" \
	    -e "s,@EXECUTABLE@,$(bin_PROGRAMS),g" \
	    -e "s,com.yourcompany.,com.copperspice.,g" \
	$(CS_HOME)/mac/Info.plist.app > $(deployapp)/Info.plist

	rm -f $(deployapp)/PkgInfo
	echo "APPL????" > $(deployapp)/PkgInfo

	touch $(deployapp)/Resources/empty.lproj
	cp -r $(CS_HOME)/mac/qt_menu.nib $(deployapp)/Resources

	cp $(top_srcdir)/resources/doxypress.icns $(deployapp)/Resources

	rsync -W -a $(patsubst %,$(CS_HOME)/lib/libCs%1.0.dylib,$(cslibs)) $(deployapp)/Frameworks

	for libname in $(patsubst %,libCs%1.0.dylib,$(cslibs)) ; do \
	\
	install_name_tool -change $(CS_HOME)/lib/$$libname \
	@executable_path/../Frameworks/$$libname  $(deployapp)/MacOS/$(bin_PROGRAMS); \
	\
	for templib in $(patsubst %,libCs%1.0.dylib,$(cslibs)) ; do \
	install_name_tool -change $(CS_HOME)/lib/$$templib \
	@executable_path/../Frameworks/$$templib  $(deployapp)/Frameworks/$$libname \
	; done \
	\
	; done

   # clean up the path for doxypress app
	for libname in $(patsubst %,libCs%1.0.dylib,$(cslibs)) ; do \
	\
	install_name_tool -change $(CS_HOME)/lib/$$libname \
	@executable_path/../Frameworks/$$libname  $(deployapp)/MacOS/doxypress \
	; done

   # generate dmg	
	hdiutil create -ov -srcfolder $(deploydir) -format UDBZ -volname $(bin_PROGRAMS) ../$(bin_PROGRAMS)-$(VERSION).dmg
	hdiutil internet-enable -yes ../$(bin_PROGRAMS)-$(VERSION).dmg
endif

if OSTYPE_WIN
deploy : all
	mkdir -p $(deploydir)
	$(LIBTOOL) --mode=install cp $(bin_PROGRAMS) $(deploydir)

	rsync -W -a --modify-window=2  $(top_srcdir)/resources/doxypress.ico $(deploydir)

	rsync -W -a --modify-window=2  $(patsubst %,$(gccpath)%.dll,$(gcclibs))  $(deploydir)
	rsync -W -a --modify-window=2  $(patsubst %,$(CS_HOME)/bin/libCs%1-0.dll,$(cslibs))  $(deploydir)
endif

# search path for source
VPATH = ../

# linking
DoxyPressApp_LDFLAGS=$(AM_LDFLAGS) $(patsubst %,-lCs%1,$(cslibs))

if OSTYPE_WIN
   # used by xp only
   DoxyPressApp_LDFLAGS+=-lnetapi32 -lmpr
endif

DoxyPressApp_SOURCES = \
   src/colors.cpp  \
   src/dialog_args.cpp \
   src/dialog_config.cpp  \
   src/dialog_editcfg.cpp  \
   src/dialog_editcfg.h   \
   src/dialog_lookup.cpp \
   src/dialog_selectcfg.cpp \
   src/dialog_xp_getdir.cpp \
   src/gui_help.cpp \
   src/gui_lookup.cpp \
   src/gui_valid.cpp  \
   src/import.cpp \
   src/json.cpp \
   src/main.cpp \
   src/mainwindow.cpp \
   src/process.cpp \
   src/recent_files.cpp \
   src/syntax.cpp \
   src/support.cpp \
   src/util.cpp

noinst_HEADERS = \
   src/colors.h \
   src/dialog_args.h \
   src/dialog_config.h \
   src/dialog_editcfg.h \
   src/dialog_lookup.h \
   src/dialog_selectcfg.h \
   src/dialog_xp_getdir.h \
   src/doxy_build_info.h \
   src/mainwindow.h \
   src/syntax.h \
   src/util.h

FORMS = \
   forms/dialog_args.ui \
   forms/dialog_config.ui \
   forms/dialog_editcfg.ui \
   forms/dialog_lookup.ui \
   forms/dialog_selectcfg.ui \
   forms/dialog_xp_getdir.ui \
   forms/mainwindow.ui


RESOURCES = doxypressapp.qrc

# use wilcard to obtain just the dependency list
qrc_doxypressapp.cpp: doxypressapp.qrc   $(wildcard ../resources/*)

# required to compile qrc
nodist_DoxyPressApp_SOURCES=qrc_doxypressapp.cpp

# distribution
distdir: $(DISTFILES)
	cd .. ; \
	test -d "$(distdir_custom)" || mkdir -p "$(distdir_custom)" ; \
	mkdir -p $(distdir_custom)/bin                              ; \
	mkdir -p $(distdir_custom)/resources                        ; \
\
	for f in $(DIST_SOURCES)  ; do install -D $$f $(distdir_custom)/$$f ; done   ;\
	for f in $(HEADERS)       ; do install -D $$f $(distdir_custom)/$$f ; done   ;\
	for f in $(FORMS)         ; do install -D $$f $(distdir_custom)/$$f ; done   ;\
\
	cp doxypressapp.qrc              $(distdir_custom)/                          ; \
	cp bin/*.am                      $(distdir_custom)/bin/                      ; \
	cp bin/Makefile.in               $(distdir_custom)/bin/                      ; \
	cp resources/*                   $(distdir_custom)/resources/                ;

	$(MAKE) $(AM_MAKEFLAGS) dist-hook

# distdir is a temp folder
dist-hook:
	cd ..  ; \
\
	cp -r bin/$(distdir)/../*   $(distdir_custom) ; \
	cd $(distdir_custom)/..                       ; \
\
	rm  -f   ../DoxyPressApp-$(VERSION).zip         ; \
	zip -r   ../DoxyPressApp-$(VERSION).zip .       ; \
	rm  -f   ../DoxyPressApp-$(VERSION).tar.bz2     ; \
	tar -cjf ../DoxyPressApp-$(VERSION).tar.bz2 .
