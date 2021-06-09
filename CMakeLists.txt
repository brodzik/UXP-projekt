cmake_minimum_required(VERSION 3.0)

project(uxp-projekt)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -Wall -Wextra -pedantic")

add_subdirectory(googletest)

include_directories(src)
include_directories(googletest/include) 

file(GLOB SERVER_FILES
    src/server/main_server.cpp
    src/server/server.cpp
)

file(GLOB CLIENT_FILES
    src/client/main_client.cpp
    src/client/client.cpp
)

file(GLOB COMMON_FILES
    src/utility.cpp
    src/common/*.cpp
)

file(GLOB TEST_FILES
    test/*.cpp
    src/common/*.cpp
    src/server/server.cpp
)

file(GLOB SYSTEM_TEST_FILES
  systemTests/*.cpp
  src/common/*.cpp
  src/server/server.cpp
  src/client/client.cpp
)

list(FILTER TEST_FILES EXCLUDE REGEX "src/server/main_server.cpp")
list(FILTER SYSTEM_TEST_FILES EXCLUDE REGEX "src/server/main_server.cpp")
list(FILTER SYSTEM_TEST_FILES EXCLUDE REGEX "src/client/main_client.cpp")

add_executable(server ${SERVER_FILES} ${COMMON_FILES})
target_link_libraries(server rt)

add_executable(client ${CLIENT_FILES} ${COMMON_FILES})
target_link_libraries(client rt)

enable_testing()

add_executable(tests ${TEST_FILES} ${COMMON_FILES})
target_link_libraries(tests PRIVATE gtest rt)
add_test(tests tests)

add_executable(systemTests ${SYSTEM_TEST_FILES} ${COMMON_FILES})
target_link_libraries(systemTests PRIVATE gtest rt)
add_test(systemTests systemTests)
