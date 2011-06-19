CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

#Change PROJECT_NAME to the name of your project
PROJECT(GAME)

if (CMAKE_BUILD_TYPE STREQUAL "")
  # CMake defaults to leaving CMAKE_BUILD_TYPE empty. This screws up
  # differentiation between debug and release builds.
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif ()

# Change to whatever version your project is at
SET( ${PROJECT_NAME}_MAJOR_VERSION 0 ) # < -- Something completely different from old version
SET( ${PROJECT_NAME}_MINOR_VERSION 1 ) # < -- Incompatable with old versions
SET( ${PROJECT_NAME}_PATCH_LEVEL 4 )   # < -- Minor fix

SET( CMAKE_MODULE_PATH    ${CMAKE_MODULE_PATH}
                          ${CMAKE_CURRENT_SOURCE_DIR}/cmake )

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

# searches for all .cpp and .h files in the current directory and add them 
# to the current project
FILE(GLOB folder_source ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)
#SOURCE_GROUP("GAME" FILES ${folder_source})
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)

# create the project (Note WIN32 here will be ignored on linux, it means set to be
# windows application instead of a console application)
ADD_EXECUTABLE(${PROJECT_NAME} ${folder_source})

IF (NOT WIN32) # Unix
    # Add compiler warnings ( Warn all, warn unused)
    ADD_DEFINITIONS( "-Wall -D__LINUX_ALSA__ -DHAVE_GETTIMEOFDAY" )
    #Declare any external dependencies that your project may have here.
    SET(Required_Packages
      ALSA
      OIS
      MYGUI
      OGRE
    )
ELSE (NOT WIN32)
    ADD_DEFINITIONS( "-Wall -D__WINDOWS_DS__" )
    #Declare any external dependencies that your project may have here.
    SET(Required_Packages
      OIS
      MYGUI
      OGRE
    )
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} winmm dsound)
ENDIF (NOT WIN32)

#this foreach loads all of the packages that you specified as required.
#It shouldn't need to be modified.
FOREACH(Package ${Required_Packages})
  FIND_PACKAGE(${Package} REQUIRED)
  IF (${Package}_FOUND)
      INCLUDE_DIRECTORIES(${${Package}_INCLUDE_DIR})
      IF (${Package}_LIBRARIES)
         TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${${Package}_LIBRARIES})
      ELSE (${Package}_LIBRARIES)
         LINK_DIRECTORIES(${${Package}_LIBRARY_DIRS})
         TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${${Package}_LIBRARY})
      ENDIF (${Package}_LIBRARIES)
  ELSE (${Package}_FOUND)
      MESSAGE(STATUS "${Package} not found")
  ENDIF(${Package}_FOUND)
ENDFOREACH(Package)
