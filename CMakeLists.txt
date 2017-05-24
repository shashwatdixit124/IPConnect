cmake_minimum_required(VERSION 3.5.1 FATAL_ERROR)
project(IPConnect)
set(IPConnect_VERSION_MAJOR 2)
set(IPConnect_VERSION_MINOR 0)
set(QT_MIN_VERSION "5.8.0")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED Core Network Widgets Quick QuickControls2)

add_subdirectory(src)
