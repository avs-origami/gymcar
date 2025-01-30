cmake_minimum_required(VERSION 3.10)
project(example)

# Add symbol visibility control
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

# Find Python and pybind11
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
execute_process(
    COMMAND ${Python3_EXECUTABLE} -c "import pybind11; print(pybind11.get_include())"
    OUTPUT_VARIABLE PYBIND11_INCLUDE_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Create shared library first
add_library(gymcar_shared SHARED
    gymcar.cpp
    gymcar.h
)

# Create Python module
add_library(gymcar MODULE
    gymcar.cpp
    gymcar.h
)

add_executable(example 
    example.cpp
)

# Include directories
target_include_directories(gymcar_shared PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PYBIND11_INCLUDE_DIR}
    ${Python3_INCLUDE_DIRS}
)

target_include_directories(gymcar PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PYBIND11_INCLUDE_DIR}
    ${Python3_INCLUDE_DIRS}
)

target_include_directories(example PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link libraries in correct order
target_link_libraries(example 
    PRIVATE 
    gymcar_shared
    ${Python3_LIBRARIES}
)

target_link_libraries(gymcar_shared PRIVATE ${Python3_LIBRARIES})
target_link_libraries(gymcar PRIVATE ${Python3_LIBRARIES})

# Set Python module properties
set_target_properties(gymcar PROPERTIES PREFIX "")
