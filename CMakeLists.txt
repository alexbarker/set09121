cmake_minimum_required(VERSION 3.9)
# Compiler flags
if (MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /EHsc" CACHE INTERNAL "" FORCE)
elseif(APPLE)
  message("hello apple")
  set(CMAKE_OSX_DEPLOYMENT_TARGET 10.9 CACHE STRING "")
endif()

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
# SFML - graphics library
add_subdirectory("lib/sfml")
#include_directories("lib/sfml/include")
set(SFML_INCS "lib/sfml/include")
link_directories("${CMAKE_BINARY_DIR}/lib/sfml/lib")

# B2D - Box2D physics library
add_subdirectory("lib/b2d/Box2D")
#include_directories("lib/b2d/Box2D/")
set(B2D_INCS "lib/b2d/Box2D/")
link_directories("${CMAKE_BINARY_DIR}/lib/sfml/lib")

#### Engine ####
file(GLOB_RECURSE RESOURCE_FILES "res/*.*")
####
file(GLOB_RECURSE SOURCE_FILES engine/src/*.cpp engine/src/*.h)
add_library(lib_engine STATIC ${SOURCE_FILES} ${RESOURCE_FILES})
target_include_directories(lib_engine INTERFACE "${CMAKE_SOURCE_DIR}/engine/src/" PUBLIC SYSTEM ${B2D_INCS} )
target_link_libraries(lib_engine lib_tile_level_loader lib_maths lib_ecm Box2D sfml-graphics sfml-audio)
## Maths lib
add_library(lib_maths INTERFACE)
target_sources(lib_maths INTERFACE "${CMAKE_SOURCE_DIR}/engine/lib_maths/maths.h")
target_include_directories(lib_maths INTERFACE "${CMAKE_SOURCE_DIR}/engine/lib_maths" SYSTEM INTERFACE ${SFML_INCS})
## Tile loader lib
file(GLOB_RECURSE SOURCE_FILES engine/lib_tile_level_loader/*.cpp engine/lib_tile_level_loader/*.h)
add_library(lib_tile_level_loader STATIC ${SOURCE_FILES} ${RESOURCE_FILES})
target_include_directories(lib_tile_level_loader INTERFACE "${CMAKE_SOURCE_DIR}/engine/lib_tile_level_loader/" )
target_link_libraries(lib_tile_level_loader lib_maths sfml-graphics)
## ECM lib
file(GLOB_RECURSE SOURCE_FILES engine/lib_ecm/*.cpp engine/lib_ecm/*.h)
add_library(lib_ecm STATIC ${SOURCE_FILES})
target_include_directories(lib_ecm INTERFACE "${CMAKE_SOURCE_DIR}/engine/lib_ecm" )
target_link_libraries(lib_ecm PRIVATE lib_maths)

#### Practical 1 ####
#### PONG ####
file(GLOB_RECURSE SOURCES practical_1/*.cpp practical_1/*.h)
add_executable(PRACTICAL_1_PONG ${SOURCES} ${RESOURCE_FILES})
target_include_directories(PRACTICAL_1_PONG SYSTEM PRIVATE ${SFML_INCS})
set(EXECUTABLES ${EXECUTABLES} PRACTICAL_1_PONG)

#### Practical 2 ####
#### Space Invaders ####
file(GLOB_RECURSE SOURCES practical_2/*.cpp practical_2/*.h)
add_executable(PRACTICAL_2_INVADERS ${SOURCES} ${RESOURCE_FILES})
target_include_directories(PRACTICAL_2_INVADERS SYSTEM PRIVATE ${SFML_INCS})
set(EXECUTABLES ${EXECUTABLES} PRACTICAL_2_INVADERS)

#### Practical 3 ####
#### Tile ####
file(GLOB_RECURSE SOURCES practical_3/*.cpp practical_3/*.h)
add_executable(PRACTICAL_3_TILE ${SOURCES} ${RESOURCE_FILES})
target_include_directories(PRACTICAL_3_TILE SYSTEM PRIVATE ${SFML_INCS})
target_link_libraries(PRACTICAL_3_TILE lib_tile_level_loader)
set(EXECUTABLES ${EXECUTABLES} PRACTICAL_3_TILE)

#### Practical 4 ####
#### Pacman ####
file(GLOB_RECURSE SOURCES practical_4/*.cpp practical_4/*.h)
add_executable(PRACTICAL_4_PACMAN ${SOURCES} ${RESOURCE_FILES})
target_include_directories(PRACTICAL_4_PACMAN SYSTEM PRIVATE ${SFML_INCS})
target_link_libraries(PRACTICAL_4_PACMAN lib_tile_level_loader lib_maths lib_ecm sfml-graphics)
set(EXECUTABLES ${EXECUTABLES} PRACTICAL_4_PACMAN)

#### Practical 5 ####
#### Physics ####
file(GLOB_RECURSE SOURCES practical_5/*.cpp practical_5/*.h)
add_executable(PRACTICAL_5_PHYSICS ${SOURCES} ${RESOURCE_FILES})
target_include_directories(PRACTICAL_5_PHYSICS SYSTEM PRIVATE ${SFML_INCS} ${B2D_INCS})
target_link_libraries(PRACTICAL_5_PHYSICS Box2D)
set(EXECUTABLES ${EXECUTABLES} PRACTICAL_5_PHYSICS)

#### Practical 6 ####
## Platformer
file(GLOB_RECURSE SOURCES practical_6/*.cpp practical_6/*.h)
file(GLOB_RECURSE CMPNTS practical_6/components/*.cpp practical_6/components/*.h)
file(GLOB_RECURSE SCENES practical_6/scenes/*.cpp practical_6/scenes/*.h)
add_executable(PRACTICAL_6_PLATFORMER ${SOURCES} ${RESOURCE_FILES})
source_group("components" FILES ${CMPNTS})
source_group("resources" FILES ${RESOURCE_FILES})
#target_include_directories(PRACTICAL_6_PLATFORMER SYSTEM PRIVATE ${SFML_INCS} ${B2D_INCS})
target_link_libraries(PRACTICAL_6_PLATFORMER lib_engine)
set(EXECUTABLES ${EXECUTABLES} PRACTICAL_6_PLATFORMER)

#### Practical 7 ####
## AI: Steering and Pathfinding
file(GLOB_RECURSE SOURCES practical_7/*.cpp practical_7/*.h)
file(GLOB_RECURSE CMPNTS practical_7/components/*.cpp practical_7/components.h)
file(GLOB_RECURSE SCENES practical_7/scenes/*.cpp practical_7/scenes/*.h)
add_executable(PRACTICAL_7_STEERING ${SOURCES} ${CMPNTS} ${SCENES} ${RESOURCE_FILES})
source_group("components" FILES ${CMPNTS})
source_group("resources" FILES ${RESOURCE_FILES})
target_link_libraries(PRACTICAL_7_STEERING lib_engine)
set(EXECUTABLES ${EXECUTABLES} PRACTICAL_7_STEERING)

#### Target for copying resources to build dir####
if(APPLE)
  add_custom_target(copy_resources ALL
          COMMAND ${CMAKE_COMMAND} -E copy_directory
          "${PROJECT_SOURCE_DIR}/res" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/res
          COMMAND ${CMAKE_COMMAND} -E copy_directory
          "${CMAKE_SOURCE_DIR}/lib/sfml/extlibs/libs-osx/Frameworks" ${CMAKE_BINARY_DIR}/lib/sfml/Frameworks
          )
else()
  add_custom_target(copy_resources ALL COMMAND ${CMAKE_COMMAND} -E copy_directory
          "${PROJECT_SOURCE_DIR}/res" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/res
          )
endif()

#### Link Dependencies ####
foreach (exe ${EXECUTABLES})
  #Set working directory to build dir
  set_target_properties(${exe} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$(Configuration)
  )

  target_link_libraries(${exe}
          lib_maths
    optimized sfml-graphics debug sfml-graphics-d
    optimized sfml-audio debug sfml-audio-d
    optimized sfml-window debug sfml-window-d
    optimized sfml-system debug sfml-system-d
    optimized sfml-graphics debug sfml-graphics-d
    #optimized sfml-main
  )

  #target_link_libraries(${exe} "${CMAKE_SOURCE_DIR}/lib/sfml/extlibs/libs-osx/Frameworks/f‌​reetype.framework/Ve‌​rsions/A/freetype")

  add_dependencies(${exe} sfml-graphics sfml-audio copy_resources)
endforeach ()