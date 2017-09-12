set(CS_VERSION 1.4)

macro(cs_copy_dll APP DLL)
   get_target_property(CS_${DLL}_LIB  CopperSpice::Cs${DLL}${CS_VERSION} LOCATION)
   install(FILES  ${CS_${DLL}_LIB}  DESTINATION .)
endmacro()

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

add_executable(DoxyPressApp ${DOXYPRESSAPP_SOURCES} )

target_include_directories(DoxyPressApp PRIVATE
   ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(DoxyPressApp
   CopperSpice::CsCore${CS_VERSION}
   CopperSpice::CsGui${CS_VERSION}
   CopperSpice::CsNetwork${CS_VERSION}
   CopperSpice::CsXml${CS_VERSION}
)

if(WIN32)
   # disable console on windows
   target_link_libraries(DoxyPressApp -lnetapi32 -lmpr -mwindows)
endif()

install(TARGETS DoxyPressApp DESTINATION .)
install(FILES   ${CMAKE_CURRENT_SOURCE_DIR}/../resources/doxypress.ico  DESTINATION .)

if(WIN32)
   cs_copy_dll(DoxyPressApp Core)
   cs_copy_dll(DoxyPressApp Gui)
   cs_copy_dll(DoxyPressApp Network)
   cs_copy_dll(DoxyPressApp Xml)
endif()

