cmake_minimum_required(VERSION 3.0...3.30.5)

project(test_gp)

set(CMAKE_CXX_STANDARD 14) # 14 needed for meshgrid
set(CMAKE_BUILD_TYPE "Release")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread -march=native -g")

file(GLOB implementation_files "../cpp/src/*.cpp")
file(GLOB header_files "../cpp/include/*.h" "../cpp/include/*.hpp")
message("headers:" "${header_files}")
message("cpps:" "${implementation_files}")

# file(GLOB debug_files "../cpp/src/get_shapes.cpp" "../cpp/src/GPShape.cpp" "../cpp/include/GPShape.hpp" "../cpp/include/get_shapes.hpp")

# set_source_files_properties(
#   ${debug_files}
#   PROPERTIES
#   COMPILE_FLAGS "-std=c++14 -march=native -pthread -Wall -W -Wextra -Wconversion -Wno-unused-parameter -Wwrite-strings"
# )

find_package(Python3 3.10 COMPONENTS Development NumPy)
message("Python PATH:" "${Python3_INCLUDE_DIRS}")
message("NumPy PATH:" "${Python3_NumPy_INCLUDE_DIRS}")
message("Python Link Options" "${Python3_LINK_OPTIONS}")
message("Python Library" "${Python3_LIBRARIES}")

find_package (Eigen3 3.3 REQUIRED NO_MODULE)
message("EIGEN PATH:" "${EIGEN3_INCLUDE_DIR}")

include_directories(
        ../cpp/include 
        ${EIGEN3_INCLUDE_DIR}
        ${Python3_INCLUDE_DIRS}
        ${Python3_NumPy_INCLUDE_DIRS}
)

add_executable(test_gp test_gp.cpp ${implementation_files} ${header_files})
target_link_libraries(test_gp ${Python3_LIBRARIES})
add_executable(test_kd test_kd.cpp ${implementation_files} ${header_files})

