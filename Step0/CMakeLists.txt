cmake_minimum_required(VERSION 3.0)

project(Step0)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_SYSTEM_PROCESSOR amd64)
set(WinLibPath "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.17763.0/um/$(CMAKE_SYSTEM_PROCESSOR)")

include_directories("C:/Program Files (x86)/Windows Kits/10/Include/10.0.17763.0/um")
link_directories($(WinLibPath))

add_executable(Step0 main.cpp SystemClass.cpp SystemClass.h InputClass.cpp InputClass.h GraphicsClass.cpp GraphicsClass.h d3dclass.cpp d3dclass.h)
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")