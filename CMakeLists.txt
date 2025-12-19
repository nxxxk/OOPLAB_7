cmake_minimum_required(VERSION 3.10)
project(Figures)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

find_package(GTest REQUIRED)

set(NPC_LIB_SOURCES
    src/BattleVisitor.cpp
    src/observers.cpp
    src/bear.cpp
    src/factory.cpp
    src/npc.cpp
    src/orc.cpp
    src/squirrel.cpp
    src/world.cpp
)

add_library(npc_lib ${NPC_LIB_SOURCES})

add_executable(DND src/main.cpp)
target_link_libraries(DND npc_lib)

add_executable(run_tests test/dnd_test.cpp)
target_link_libraries(run_tests npc_lib GTest::gtest GTest::gtest_main pthread)

include(GoogleTest)
gtest_discover_tests(run_tests)
