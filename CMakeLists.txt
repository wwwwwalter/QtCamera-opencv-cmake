cmake_minimum_required(VERSION 3.28)
project(qt-camera-opencv)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        OpenGLWidgets
        Multimedia
        MultimediaWidgets
        REQUIRED)



# vcpkg工具链
set(CMAKE_TOOLCHAIN_FILE "D:/vcpkg/scripts/buildsystems/vcpkg.cmake")
include(${CMAKE_TOOLCHAIN_FILE})

find_package(OpenCV REQUIRED)



add_executable(qt-camera-opencv
        main.cpp
        mainwindow.cpp
        mainwindow.h
        settings.h settings.cpp
        frameprocessor.h frameprocessor.cpp
        framehandle.h framehandle.cpp)
target_link_libraries(qt-camera-opencv PRIVATE
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt6::Multimedia
        Qt6::MultimediaWidgets
        Qt6::OpenGLWidgets
        ${OpenCV_LIBS}
)
