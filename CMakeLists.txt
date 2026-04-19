cmake_minimum_required(VERSION 3.21)
project(nexgen_qt_sys VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network)

add_library(nexgen_qt_sys STATIC)
add_library(nexgen::qt_sys ALIAS nexgen_qt_sys)

target_sources(nexgen_qt_sys
  PRIVATE
    src/Settings.cpp
    src/IpcClient.cpp
    src/IpcServer.cpp
    src/HotkeyManager_win.cpp
  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS include
    FILES
      include/nexgen/sys/Settings.h
      include/nexgen/sys/ipc/IpcClient.h
      include/nexgen/sys/ipc/IpcServer.h
      include/nexgen/sys/hotkeys/HotkeyManager.h
)

target_link_libraries(nexgen_qt_sys PUBLIC Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Network)

target_include_directories(nexgen_qt_sys PUBLIC include)

if (WIN32)
  target_compile_definitions(nexgen_qt_sys PRIVATE NXG_WIN=1)
else()
  target_compile_definitions(nexgen_qt_sys PRIVATE NXG_WIN=0)
endif()
