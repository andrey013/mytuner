###############################################################################
# MyGUI
###############################################################################

include( "${CMAKE_MODULE_PATH}/FindPkgMacros.cmake" )

getenv_path(MYGUI_DIR)

findpkg_begin ( "MYGUI" )

set ( MYGUI_LIBRARY_NAMES MyGUIEngine )
get_debug_names ( MYGUI_LIBRARY_NAMES )

find_path ( MYGUI_INCLUDE_DIR
  MyGUI.h PATHS
  "${ENV_MYGUI_DIR}/include/MYGUI"
  "/usr/include/MYGUI"
  "/usr/local/include/MYGUI"
  NO_DEFAULT_PATH )

find_library ( MYGUI_LIBRARY_REL
  ${MYGUI_LIBRARY_NAMES} PATHS
  "${ENV_MYGUI_DIR}/lib/Release"
  "${ENV_MYGUI_DIR}/lib/release"
  "/usr/lib"
  "/usr/local/lib"
  NO_DEFAULT_PATH )
  
find_library ( MYGUI_LIBRARY_DBG
  ${MYGUI_LIBRARY_NAMES_DBG} PATHS
  "${ENV_MYGUI_DIR}/lib/Debug"
  "${ENV_MYGUI_DIR}/lib/debug"
  "/usr/lib"
  "/usr/local/lib"
  NO_DEFAULT_PATH )

make_library_set ( MYGUI_LIBRARY )

findpkg_finish ( "MYGUI" )

###############################################################################
# MyGUI OgrePlatform
###############################################################################

findpkg_begin ( "MYGUI_PLATFORM" )

set ( MYGUI_PLATFORM_LIBRARY_NAMES "MyGUI.OgrePlatform" )
get_debug_names ( MYGUI_PLATFORM_LIBRARY_NAMES )

find_path ( MYGUI_PLATFORM_INCLUDE_DIR
  MyGUI_OgrePlatform.h PATHS
  "${ENV_MYGUI_DIR}/include/MYGUI"
  "/usr/include/MYGUI"
  "/usr/local/include/MYGUI"
  "/usr/include/OGRE"
  "/usr/local/include/OGRE"
  NO_DEFAULT_PATH )

find_library ( MYGUI_PLATFORM_LIBRARY_REL
  ${MYGUI_PLATFORM_LIBRARY_NAMES} PATHS
  "${ENV_MYGUI_DIR}/lib/Release"
  "${ENV_MYGUI_DIR}/lib/release"
  "/usr/lib"
  "/usr/local/lib"
  NO_DEFAULT_PATH )
 
find_library ( MYGUI_PLATFORM_LIBRARY_DBG
  ${MYGUI_PLATFORM_LIBRARY_NAMES_DBG} PATHS
  "${ENV_MYGUI_DIR}/lib/Debug"
  "${ENV_MYGUI_DIR}/lib/debug"
  "/usr/lib"
  "/usr/local/lib"
  NO_DEFAULT_PATH )

make_library_set ( MYGUI_PLATFORM_LIBRARY )

set ( MYGUI_PLATFORM_FOUND TRUE )
#set ( MYGUI_FOUND TRUE )
if ( NOT MYGUI_PLATFORM_LIBRARY OR NOT MYGUI_PLATFORM_INCLUDE_DIR )
  message ( FATAL_ERROR "MyGUI OgrePlatform library or include directory could not be located" )
  set ( MYGUI_PLATFORM_FOUND FALSE )
#  set ( MYGUI_FOUND FALSE )
endif ()

if ( NOT MYGUI_PLATFORM_FOUND  )
  set ( MYGUI_LIBRARIES "MYGUI_PLATFORM-NOTFOUND")
else ()
  set ( MYGUI_LIBRARIES ${MYGUI_PLATFORM_LIBRARY} ${MYGUI_LIBRARY} )
    
  set ( MYGUI_INCLUDE_DIRS
    "${MYGUI_INCLUDE_DIR}"
    "${MYGUI_PLATFORM_INCLUDE_DIR}" )
endif ()

findpkg_finish ( MYGUI_PLATFORM )
