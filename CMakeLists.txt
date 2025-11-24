cmake_minimum_required(VERSION 3.5)

project(FINGERPRINTS_2025)

# set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_BUILD_TYPE Debug)

#set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Wshadow -Wuninitialized -Wcast-align -Wconversion -Wnull-dereference -Wdouble-promotion -Wformat=2 -Wredundant-decls -Wparentheses -Wold-style-cast -Woverloaded-virtual -Wsign-conversion -Wimplicit-fallthrough -Wmissing-declarations -Winline -Woverloaded-virtual -Wunreachable-code -Wctor-dtor-privacy")


include_directories(${PROJECT_SOURCE_DIR})

file(GLOB PROJECT_SOURCES
     "src/*.cpp"
     #Décommenter pour avoir accès au headers.
     #"src/*.hpp"
      )

add_executable (run  ${PROJECT_SOURCES})

target_link_libraries(run)

