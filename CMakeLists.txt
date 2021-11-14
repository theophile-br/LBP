cmake_minimum_required(VERSION 3.0)
project(Rock_Class_AI)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories("src/include")
include_directories("src/utils/include")
add_executable(test src/test.cpp src/utils/lbp.cpp src/utils/formula.cpp src/utils/utils.cpp)
add_executable(train src/train.cpp src/utils/lbp.cpp src/utils/utils.cpp)
add_executable(guess src/guess.cpp src/utils/lbp.cpp src/utils/formula.cpp src/utils/utils.cpp)
add_executable(prepare src/prepare.cpp src/utils/lbp.cpp src/utils/utils.cpp)

target_link_libraries(test ${OpenCV_LIBS})
target_link_libraries(train ${OpenCV_LIBS})
target_link_libraries(guess ${OpenCV_LIBS})
target_link_libraries(prepare ${OpenCV_LIBS})

file(COPY python-scripts DESTINATION ${CMAKE_BINARY_DIR})
