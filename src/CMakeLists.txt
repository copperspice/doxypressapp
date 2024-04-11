list(APPEND DOXYPRESSAPP_INCLUDES
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

list(APPEND DOXYPRESSAPP_SOURCES
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
COPPERSPICE_RESOURCES(
   ${CMAKE_CURRENT_SOURCE_DIR}/../doxypressapp.qrc
)

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

target_include_directories(DoxyPressApp
   PRIVATE
   ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(DoxyPressApp
   CopperSpice::CsCore
   CopperSpice::CsGui
   CopperSpice::CsNetwork
   CopperSpice::CsXml
)

if (CMAKE_SYSTEM_NAME MATCHES "Darwin")
   set_target_properties(DoxyPressApp PROPERTIES OUTPUT_NAME doxypressapp)

   install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../resources/doxypress.icns  DESTINATION .)

elseif(CMAKE_SYSTEM_NAME MATCHES "(Linux|OpenBSD|FreeBSD|NetBSD|DragonFly)")
   set_target_properties(DoxyPressApp PROPERTIES OUTPUT_NAME doxypressapp)

   install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../resources/doxypress.png  DESTINATION .)

elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
   target_link_libraries(DoxyPressApp
      netapi32
      mpr
   )

   if (MSVC)
      # disables the console window
      target_link_options(DoxyPressApp
         PRIVATE
         /subsystem:windows
         /entry:mainCRTStartup
      )

   else()
      # disables the console window
      target_link_libraries(DoxyPressApp
         -mwindows
      )

   endif()

   install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../resources/doxypress.ico  DESTINATION .)

endif()

install(TARGETS DoxyPressApp DESTINATION .)

cs_copy_library(CsCore)
cs_copy_library(CsGui)
cs_copy_library(CsNetwork)
cs_copy_library(CsXml)

# installs the platform Gui plugin
cs_copy_plugins(CsGui)
