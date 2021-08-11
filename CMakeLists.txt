cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(example-app)

find_package(Torch REQUIRED)
link_libraries(${TORCH_LIBRARIES})
find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})
add_subdirectory(benchmark)
link_libraries(benchmark::benchmark)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")

add_executable(example-app-baseline example-app.cpp)
target_compile_definitions(example-app-baseline PRIVATE _BASELINE)
target_link_libraries(example-app-baseline PRIVATE ${TORCH_LIBRARIES})

add_executable(example-app-templated example-app.cpp)
target_link_libraries(example-app-templated PRIVATE ${TORCH_LIBRARIES})

add_executable(example-app-boxed example-app.cpp)
target_compile_definitions(example-app-boxed PRIVATE _USE_BOXED)
target_link_libraries(example-app-boxed PRIVATE ${TORCH_LIBRARIES})

set_property(GLOBAL PROPERTY CXX_STANDARD 14)
