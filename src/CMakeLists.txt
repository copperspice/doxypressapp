#
# Copyright (C) 2014-2017 Barbara Geller & Ansel Sermersheim
# All rights reserved.    
#
# Permission to use, copy, modify, and distribute this software and its
# documentation under the terms of the GNU General Public License version 2
# is hereby granted. No representations are made about the suitability of
# this software for any purpose. It is provided "as is" without express or
# implied warranty. See the GNU General Public License for more details.
#
# Documents produced by DoxyPress are derivative works derived from the
# input used in their production; they are not affected by this license.

# (todo: enhance) hard coded for testing
set(CS_INCLUDE_DIR "z:/cs5_x_lib/include" 
                   "z:/cs5_x_lib/include/QtCore"
                   "z:/cs5_x_lib/include/QtGui" 
                   "z:/cs5_x_lib/include/QtNetwork" 
                   "z:/cs5_x_lib/include/QtXml"
) 

include_directories( ${CMAKE_SOURCE_DIR}/src
                     ${CS_INCLUDE_DIR}                     
)

check_include_file(windows.h HAVE_WINDOWS_H)

if (HAVE_WINDOWS_H)
   add_definitions(-DHAVE_WINDOWS_H)
endif()

set(DOXYPRESSAPP_INCLUDES
   ${CMAKE_CURRENT_SOURCE_DIR}/colors.h
   ${CMAKE_CURRENT_SOURCE_DIR}/dialog_args.h
   ${CMAKE_CURRENT_SOURCE_DIR}/dialog_config.h
   ${CMAKE_CURRENT_SOURCE_DIR}/dialog_editcfg.h
   ${CMAKE_CURRENT_SOURCE_DIR}/dialog_find.h
   ${CMAKE_CURRENT_SOURCE_DIR}/dialog_lookup.h
   ${CMAKE_CURRENT_SOURCE_DIR}/dialog_selectcfg.h
   ${CMAKE_CURRENT_SOURCE_DIR}/dialog_xp_getdir.h
   ${CMAKE_CURRENT_SOURCE_DIR}/doxy_build_info.h
   ${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.h
   ${CMAKE_CURRENT_SOURCE_DIR}/syntax.h
   ${CMAKE_CURRENT_SOURCE_DIR}/util.h
)

set(DOXYPRESSAPP_SOURCES
   ${CMAKE_CURRENT_SOURCE_DIR}/colors.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/dialog_args.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/dialog_config.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/dialog_editcfg.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/dialog_find.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/dialog_lookup.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/dialog_selectcfg.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/dialog_xp_getdir.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/gui_help.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/gui_lookup.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/gui_valid.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/import.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/json.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/process.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/recent_files.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/support.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/syntax.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/util.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/../forms/dialog_args.ui
   ${CMAKE_CURRENT_SOURCE_DIR}/../forms/dialog_config.ui
   ${CMAKE_CURRENT_SOURCE_DIR}/../forms/dialog_editcfg.ui
   ${CMAKE_CURRENT_SOURCE_DIR}/../forms/dialog_find.ui
   ${CMAKE_CURRENT_SOURCE_DIR}/../forms/dialog_lookup.ui
   ${CMAKE_CURRENT_SOURCE_DIR}/../forms/dialog_selectcfg.ui
   ${CMAKE_CURRENT_SOURCE_DIR}/../forms/dialog_xp_getdir.ui
   ${CMAKE_CURRENT_SOURCE_DIR}/../forms/mainwindow.ui
   qrc_doxypressapp.cpp
)

# run rcc to generate qrc output
COPPERSPICE_RESOURCES(${CMAKE_CURRENT_SOURCE_DIR}/../doxypressapp.qrc)

# run uic to generate source
COPPERSPICE_RESOURCES( 
    ${CMAKE_CURRENT_SOURCE_DIR}/../forms/dialog_args.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/../forms/dialog_config.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/../forms/dialog_editcfg.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/../forms/dialog_find.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/../forms/dialog_lookup.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/../forms/dialog_selectcfg.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/../forms/dialog_xp_getdir.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/../forms/mainwindow.ui
)

add_executable(doxypressapp ${DOXYPRESSAPP_SOURCES} )

# (todo: enhance)
# target_link_libraries(doxypressapp CopperSpice::CsCore CopperSpice::CsGui
#            CopperSpice::CsNetwork CopperSpice::CsXml  )

target_link_libraries(doxypressapp

   # FIXME - hard coded
   "z:/cs5_x_lib/lib/libCsCore1.4.dll.a"
   "z:/cs5_x_lib/lib/libCsGui1.4.dll.a"
   "z:/cs5_x_lib/lib/libCsNetwork1.4.dll.a"
   "z:/cs5_x_lib/lib/libCsXml1.4.dll.a"
   -lnetapi32
   -lmpr
)

if(WIN32)
   # additional libs for windows suppport, disable console on windows
   target_link_libraries(doxypressapp -lnetapi32 -lmpr -mwindows)
endif()

install(TARGETS doxypressapp DESTINATION .)
install(FILES   ${CS_LIBS}   DESTINATION .)
install(FILES   ${CMAKE_CURRENT_SOURCE_DIR}/../resources/doxypress.ico  DESTINATION .)

if(WIN32)
   # copy MingW files, windows only
   install(FILES ${GCC_LIBS}  DESTINATION .)
endif()
