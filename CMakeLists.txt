cmake_minimum_required(VERSION 3.9)
# Require modern C++
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Games_Engineering)

#### Setup Directories ####
#Main output directory
SET(OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")
# Output all DLLs from all libs into main build folder
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})

#### Add External Dependencies ####
add_subdirectory("lib/sfml")
set(SFML_INCS "lib/sfml/include")
link_directories("${CMAKE_BINARY_DIR}/lib/sfml/lib")

# B2D - Box2D physics library
add_subdirectory("lib/b2d/Box2D")
#include_directories("lib/b2d/Box2D/")
set(B2D_INCS "lib/b2d/Box2D/")
link_directories("${CMAKE_BINARY_DIR}/lib/sfml/lib")

#### Engine ####
#file(GLOB_RECURSE RESOURCE_FILES "res/*.*")


#### Practical 1 ####
#### PONG ####
file(GLOB_RECURSE SOURCES practical_1/*.cpp practical_1/*.h)
add_executable(PRACTICAL_1_PONG ${SOURCES})
target_include_directories(PRACTICAL_1_PONG PRIVATE ${SFML_INCS})
target_link_libraries(PRACTICAL_1_PONG sfml-graphics)

#### Practical 2 ####
#### Space Invaders ####
file(GLOB_RECURSE SOURCES practical_2/*.cpp practical_2/*.h)
add_executable(PRACTICAL_2_INVADERS ${SOURCES})
target_include_directories(PRACTICAL_2_INVADERS SYSTEM PRIVATE ${SFML_INCS})
target_link_libraries(PRACTICAL_2_INVADERS sfml-graphics)

## COPY RESOURCES ##
add_custom_target(copy_resources ALL COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/res" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/res)
add_dependencies(PRACTICAL_2_INVADERS copy_resources)

#### Practical 3 ####
#### Tile ####
file(GLOB_RECURSE SOURCES practical_3/*.cpp practical_3/*.h)
add_executable(PRACTICAL_3_TILE ${SOURCES})
target_include_directories(PRACTICAL_3_TILE SYSTEM PRIVATE ${SFML_INCS})
target_link_libraries(PRACTICAL_3_TILE lib_tile_level_loader lib_maths sfml-graphics)

#### Practical 4 ####
#### Pacman ####
file(GLOB_RECURSE SOURCES practical_4/*.cpp practical_4/*.h)
add_executable(PRACTICAL_4_PACMAN ${SOURCES})
target_include_directories(PRACTICAL_4_PACMAN SYSTEM PRIVATE ${SFML_INCS})
target_link_libraries(PRACTICAL_4_PACMAN lib_tile_level_loader lib_maths lib_ecm sfml-graphics)

## COPY RESOURCES ##
add_custom_target(copy_resources_pacman ALL COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/res" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/res)
add_dependencies(PRACTICAL_4_PACMAN copy_resources_pacman)

#### Practical 5 ####
#### Physics ####
file(GLOB_RECURSE SOURCES practical_5/*.cpp practical_5/*.h)
add_executable(PRACTICAL_5_PHYSICS ${SOURCES})
target_include_directories(PRACTICAL_5_PHYSICS SYSTEM PRIVATE ${SFML_INCS} ${B2D_INCS})
target_link_libraries(PRACTICAL_5_PHYSICS Box2D)
set(EXECUTABLES ${EXECUTABLES} PRACTICAL_5_PHYSICS)

## COPY RESOURCES ##
add_custom_target(copy_resources_physics ALL COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/res" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/res)
add_dependencies(PRACTICAL_5_PHYSICS copy_resources_physics)

## Tile Loader lib ##
file(GLOB_RECURSE SOURCE_FILES lib_tile_level_loader/*.cpp lib_tile_level_loader/*.h)
add_library(lib_tile_level_loader STATIC ${SOURCE_FILES})
target_include_directories(lib_tile_level_loader INTERFACE "${CMAKE_SOURCE_DIR}/lib_tile_level_loader/" ${SFML_INCS})
target_link_libraries(lib_tile_level_loader lib_maths sfml-graphics)

## Maths lib ##
add_library(lib_maths INTERFACE)
target_sources(lib_maths INTERFACE "${CMAKE_SOURCE_DIR}/lib_maths/maths.h")
target_include_directories(lib_maths INTERFACE "${CMAKE_SOURCE_DIR}/lib_maths" SYSTEM INTERFACE ${SFML_INCS})

## ECM lib ##
file(GLOB_RECURSE SOURCE_FILES lib_ecm/*.cpp lib_ecm/*.h)
add_library(lib_ecm STATIC ${SOURCE_FILES})
target_include_directories(lib_ecm INTERFACE "${CMAKE_SOURCE_DIR}/lib_ecm")
target_link_libraries(lib_ecm PRIVATE lib_maths)