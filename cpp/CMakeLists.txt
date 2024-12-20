set(LIBRARY_NAME gpis)

find_package(Boost 1.55 REQUIRED)

file(GLOB implementation_files "src/*.cpp")
file(GLOB header_files "include/*.h" "include/*.hpp")
message("headers:" "${header_files}")

add_library(${LIBRARY_NAME} SHARED ${implementation_files})

find_package(Python3 3.10 COMPONENTS Development NumPy)
message("Python PATH:" "${Python3_INCLUDE_DIRS}")
message("NumPy PATH:" "${Python3_NumPy_INCLUDE_DIRS}")

find_package (Eigen3 3.3 REQUIRED NO_MODULE)
message("EIGEN PATH:" "${EIGEN3_INCLUDE_DIR}")

include_directories(
  ${Boost_INCLUDE_DIRS}
  ${Python3_INCLUDE_DIRS}
  ${Python3_NumPy_INCLUDE_DIRS}
  ${EIGEN3_INCLUDE_DIR}
)

set(INCLUDE_DIRS include)
include_directories(${INCLUDE_DIRS})

# install library
pods_install_headers(${header_files} DESTINATION ${LIBRARY_NAME})
pods_install_libraries(${LIBRARY_NAME})
pods_install_pkg_config_file(${LIBRARY_NAME}
  LIBS -l${LIBRARY_NAME}
  VERSION 0.0.1)
