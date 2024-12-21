cmake_minimum_required(VERSION 3.15...3.26)
project(${SKBUILD_PROJECT_NAME} LANGUAGES CXX)

file(GLOB sources "src/*.cpp")

find_package(Boost 1.55 REQUIRED)
message("Boost PATH:" "${Boost_INCLUDE_DIRS}")

find_package (Eigen3 3.3 REQUIRED NO_MODULE)
message("EIGEN PATH:" "${EIGEN3_INCLUDE_DIR}")

find_package(PythonExtensions REQUIRED)

include_directories(
  ${Boost_INCLUDE_DIRS}
  ${EIGEN3_INCLUDE_DIR}
)

add_library(_gpis MODULE ${sources})

python_extension_module(_gpis)
install(TARGETS _gpis LIBRARY DESTINATION gpis)
