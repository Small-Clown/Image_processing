cmake_minimum_required(VERSION 3.13)

set(PROJECT_ID image_processing)
project(${PROJECT_ID})

include_directories(include)

if (WIN32)
    add_compile_options(-Wno-multichar)
endif (WIN32)

set(MAIN ${PROJECT_ID}__main)
set(SOURCE_FILES src/improc.cpp src/imqueue.cpp src/bitmap.c)

add_executable(${MAIN} ${SOURCE_FILES} main.cpp)

set(SOURCES_FILES_TESTS_MAIN test/main_gtest.cpp test/test_io.cpp test/test_aux.cpp test/test_improc.cpp test/test_imqueue.cpp)

if(EXISTS ${PROJECT_SOURCE_DIR}/test)
    set(EXEC_TEST ${PROJECT_ID}__test)
    add_executable(${EXEC_TEST} ${SOURCE_FILES} ${SOURCES_FILES_TESTS_MAIN} ${SOURCE_FILES_TESTS_AUX})

    set(GTEST ../googletest-master)

    target_include_directories(${EXEC_TEST} PUBLIC
            ${GTEST}/googlemock/include
            ${GTEST}/googletest/include
            )

    add_subdirectory(googletest-master)

    target_link_libraries(${EXEC_TEST} gmock)
endif()