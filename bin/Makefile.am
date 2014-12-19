include copperspice.am

# path to gcc (windows)
gccpath=/c/MingW/bin

# list of gcc libraries (windows)
gcclibs=libgcc_s_sjlj-1  libstdc++-6  libwinpthread-1

# list of CopperSpice libraries
cslibs=Core Gui Xml

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
bin_PROGRAMS = CS_Doxywizard

# copy required CopperSpice and gcc files for deployment
.PHONY: deploy

if OSTYPE_LINUX
endif

if OSTYPE_DARWIN
endif

if OSTYPE_WIN
deploy : all
	mkdir -p $(deploydir)

	$(LIBTOOL) --mode=install cp $(bin_PROGRAMS) $(deploydir)

#	rsync -W -a --modify-window=2  $(top_srcdir)/resources/diamond.ico $(deploydir)

	rsync -W -a --modify-window=2  $(patsubst %,$(gccpath)/%.dll,$(gcclibs))  $(deploydir)
	rsync -W -a --modify-window=2  $(patsubst %,$(CS_HOME)/bin/libCs%1-0.dll,$(cslibs))  $(deploydir)
endif

# search path for source
VPATH = ../

# linking
CS_Doxywizard_LDFLAGS=$(AM_LDFLAGS) $(patsubst %,-lCs%1,$(cslibs))


CS_Doxywizard_SOURCES = \
   src/doxywizard.cpp \
   src/expert.cpp \
   src/main.cpp \
   src/wizard.cpp \
   src/inputbool.cpp \
   src/inputstring.cpp  \
   src/inputint.cpp  \
   src/inputstrlist.cpp  \
   src_gen/configdoc.cpp  \
   src_gen/config_doxyw.cpp


RESOURCES = cs_doxywizard.qrc

# use wilcard to obtain just the dependency list
qrc_cs_doxywizard.cpp: cs_doxywizard.qrc   $(wildcard ../resources/*)

# required to compile qrc
nodist_CS_Doxywizard_SOURCES=qrc_cs_doxywizard.cpp