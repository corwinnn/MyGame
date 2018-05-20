cmake_minimum_required(VERSION 3.1)
project(MyGame)
unset(WITH_TESTS CACHE)
unset(DEBUG CACHE)
option(BUILD_DOC "Build documentation" ON)

# check if Doxygen is installed
find_package(Doxygen)
if (DOXYGEN_FOUND)
    # set input and output files
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    # request to configure the file
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message("Doxygen build started")

    # note the option ALL which allows to build the docs together with the application
    add_custom_target( doc_doxygen ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM )
else (DOXYGEN_FOUND)
    message("Doxygen need to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND)
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
set(CMAKE_CXX_STANDARD 11)
set(SOURCES_LIST1 "gameOn.cpp")
include_directories(Race Units World Buildings Map Test)
set(SOURCES_LIST2 "testing.cpp")
if (WITH_TEST)
    add_executable(runTests ${SOURCES_LIST2})
    target_link_libraries(runTests ${GTEST_LIBRARIES} pthread)
elseif(DEBUG)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
    add_executable(MyGame ${SOURCES_LIST1})
else()
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")
    add_executable(MyGame ${SOURCES_LIST1})
endif()