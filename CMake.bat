@echo off

set CMAKE=../cmake-2.8.2-win32-x86

set MINGW=../MinGW

set MYGUI_DIR=../MYGUI-3.0.1-win32

rem set SDLDIR=../SDL-1.2.14

set OGRE_HOME=../OGRE-1.7.1-win32

rem set BOOST_ROOT=../OgreSDK_mingw_v1-7-1/boost_1_42

rem set BUILD=RelWithDebInfo
rem set BUILD=Release
rem set BUILD=Debug

set PATH=%PATH%;%CMAKE%/bin;%MINGW%/bin

rem cmake.exe -G "MinGW Makefiles" . -DCMAKE_BUILD_TYPE="%BUILD%"

rem mingw32-make.exe

cmake-gui

echo All done.
pause
