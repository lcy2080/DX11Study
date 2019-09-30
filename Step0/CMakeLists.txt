cmake_minimum_required(VERSION 3.0)

project(Step0)

set(CMAKE_CXX_STANDARD 17)

if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    #add_compile_options(/arch:SSE2)
endif()

cmake_host_system_information(RESULT os_name QUERY OS_NAME)
cmake_host_system_information(RESULT os_version QUERY OS_VERSION)

message("OS NAME : " ${os_name})
message("OS VERSION : " ${os_version})
message("Host System Processor : " ${CMAKE_HOST_SYSTEM_PROCESSOR})

if(os_name STREQUAL "Windows")
    message("Current OS : " ${os_name})
    if(CMAKE_SYSTEM_PROCESSOR STREQUAL "AMD64")
        message("Target System Processor : " ${CMAKE_SYSTEM_PROCESSOR})

        link_directories(
                #"C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Tools/MSVC/14.16.27023/lib/x64"
                #"C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Tools/MSVC/14.16.27023/atlmfc/lib/x64"
                #"C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Auxiliary/VS/lib/x64"
                "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.17763.0/um/x64"
                "C:/Program Files (x86)/Windows Kits/10/lib/10.0.17763.0/ucrt/x64"
                "C:/Program Files (x86)/Windows Kits/NETFXSDK/4.7.2/lib/um/x64"
                "C:/Program Files (x86)/Windows Kits/NETFXSDK/4.7.2/Lib/um/x64")

        include_directories(
                #"C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Tools/MSVC/14.16.27023/include"
                #"C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Tools/MSVC/14.16.27023/atlmfc/include"
                #"C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Auxiliary/VS/include"
                "C:/Program Files (x86)/Windows Kits/NETFXSDK/4.7.2/Include/um"
                "C:/Program Files (x86)/Windows Kits/10/Include/10.0.17763.0/um"
                "C:/Program Files (x86)/Windows Kits/10/Include/10.0.17763.0/shared"
                "C:/Program Files (x86)/Windows Kits/10/Include/10.0.17763.0/cppwinrt"
                "C:/Program Files (x86)/Windows Kits/10/Include/10.0.17763.0/ucrt"
                "C:/Program Files (x86)/Windows Kits/10/Include/10.0.17763.0/winrt")
    elseif()
        message(ERROR "Cannot get Windows and DX Lib.")
    endif()
endif()


add_executable(Step0 main.cpp SystemClass.cpp SystemClass.h InputClass.cpp InputClass.h GraphicsClass.cpp GraphicsClass.h d3dclass.cpp d3dclass.h AlignedAllocationPolicy.h ColorShaderClass.cpp ColorShaderClass.h ModelClass.cpp ModelClass.h CameraClass.cpp CameraClass.h TextureShaderClass.cpp TextureShaderClass.h TextureClass.cpp TextureClass.h LightShaderClass.cpp LightShaderClass.h LightClass.cpp LightClass.h)

if(os_name STREQUAL "Windows")
    target_link_libraries(Step0 winmm.lib)
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
endif()