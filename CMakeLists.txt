cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

# set the project name
project(eris)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

# Build Library and Executable
################################

# Set gdb debug
################################
set(CMAKE_BUILD_TYPE Debug)

# Set folders for bin and lib files
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Add Library for ERIS sources (thats the way to do it)
add_library(erisLib "") # necessary because that command requires a list of source files, even if that list is empty

# Add subdirectories with separate CmakeLists files specifying the source files
add_subdirectory(src/)
add_subdirectory(src/utils/)
add_subdirectory(src/graph/)
add_subdirectory(src/eval/)
add_subdirectory(src/widgets/)
add_subdirectory(src/widgets/main_window/)

# Set the resources and tests folder
set(RESDIR src/resources/)
set(TESTDIR test/)

# configure_file(${SRCDIR}eris_config.h.in ${SRCDIR}eris_config.h)


# Filter resources
file(GLOB RESOURCES "${RESDIR}*.qrc")

# add the executable [OLD]
# file(GLOB SOURCES "${SRCDIR}*")
# add_executable(eris ${SOURCES} ${RESOURCES})

# add external libraries
find_package(Qt5 COMPONENTS Core Gui Widgets PrintSupport Charts Svg REQUIRED)

# Link erisLib and dependencies
target_link_libraries(erisLib pthread Qt5::Charts Qt5::Core Qt5::Gui Qt5::Widgets Qt5::PrintSupport Qt5::Svg)

# Build executable from eris main routine, include resources
add_executable(eris src/main.cpp ${RESOURCES})

# Final linking
target_link_libraries(eris erisLib)

# GTest
################################
# FIX THIS
if(false)
include(GoogleTest)

file(GLOB TESTS "${TESTDIR}*.cpp")

list(FILTER SOURCES EXCLUDE REGEX "src/main.cpp")
add_executable(Tests ${TESTS} ${SOURCES} ${RESOURCES})

#add_library(CodeToTest ${SOURCES} ${RESOURCES})
	
target_link_libraries(Tests gtest pthread Qt5::Core Qt5::Gui Qt5::Widgets Qt5::PrintSupport Qt5::Svg) 
gtest_discover_tests(Tests)
endif()
