include copperspice.am

# path to gcc (windows)
gccpath=/c/MingW/bin

# list of gcc libraries (windows)
gcclibs=libgcc_s_sjlj-1  libstdc++-6  libwinpthread-1

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

   # hard coded line to copy doxypress app into this dmg
	cp $(top_srcdir)/../DoxyPress/deploy/doxypress.app/Contents/MacOS/doxypress  $(deployapp)/MacOS

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

   # generate dmg	
	hdiutil create -ov -srcfolder $(deploydir) -format UDBZ -volname $(bin_PROGRAMS) ../$(bin_PROGRAMS).dmg
	hdiutil internet-enable -yes ../$(bin_PROGRAMS).dmg
endif

if OSTYPE_WIN
deploy : all
	mkdir -p $(deploydir)

	$(LIBTOOL) --mode=install cp $(bin_PROGRAMS) $(deploydir)

	rsync -W -a --modify-window=2  $(top_srcdir)/resources/doxypress.ico $(deploydir)

	rsync -W -a --modify-window=2  $(patsubst %,$(gccpath)/%.dll,$(gcclibs))  $(deploydir)
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
   src/dialog_config.cpp  \
   src/dialog_config.h   \
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


FORMS = \
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