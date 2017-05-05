cmake_minimum_required(VERSION 3.8)

project(tutcatalog)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Concurrent REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Qml REQUIRED)
find_package(Qt5Svg REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Xml REQUIRED)

option(WITH_TESTS "build some tests" OFF)
option(WITH_DEMOS "build some demo apps" OFF)

if(${WITH_TESTS})
    find_package(Qt5Test REQUIRED)
endif()

# global definitions
add_definitions(-DQTQMLTRICKS_NO_PREFIX_ON_GETTERS)

# define variables used in config.h.in
set(MY_COPYRIGHT "(c) 2016-2017 Ioan Calin")
set(MY_COMPANY "Ioan Calin")
set(MY_VERSION "0.0.1.0")
set(MY_DOMAIN "borco.ro")
set(MY_APPLICATION "tutcatalog")

# Download automatically, you can also just copy the conan.cmake file
if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
   message(STATUS "Downloading conan.cmake from https://github.com/memsharded/cmake-conan")
   file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/master/conan.cmake"
                 "${CMAKE_BINARY_DIR}/conan.cmake")
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_cmake_run(CONANFILE conanfile.txt
                BASIC_SETUP CMAKE_TARGETS
                BUILD missing)

add_custom_target(others SOURCES
    README.md
    LICENSE
    .gitignore
    docs/tutcatalog-edit-mode.png
    docs/tutcatalog-view-mode.png
    )

add_subdirectory(src)
