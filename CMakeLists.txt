cmake_minimum_required(VERSION 3.30...3.30.5)
project(${SKBUILD_PROJECT_NAME} LANGUAGES CXX)

file(GLOB sources "src/*.cpp")

find_package(Boost 1.70 REQUIRED)
message("Boost PATH:" "${Boost_INCLUDE_DIRS}")

find_package (Eigen3 3.3 REQUIRED NO_MODULE)
message("EIGEN PATH:" "${EIGEN3_INCLUDE_DIR}")

include_directories(
  ${Boost_INCLUDE_DIRS}
  ${EIGEN3_INCLUDE_DIR}
)

find_package(Python COMPONENTS Interpreter Development.Module REQUIRED)
python_add_library(_gpis MODULE WITH_SOABI ${sources})

install(TARGETS _gpis LIBRARY DESTINATION gpis)
