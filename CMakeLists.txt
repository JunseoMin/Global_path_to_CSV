cmake_minimum_required(VERSION 3.10)
project(PcdToCsv)

# C++ 표준 설정
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# PCL 경로 설정
find_package(PCL 1.8 REQUIRED)

# OpenCV 경로 설정
find_package(OpenCV REQUIRED)

include_directories(${PCL_INCLUDE_DIRS})
include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})

# 실행 파일 추가
add_executable(PcdToCsv src/path_to_csv.cpp)

# 링크 라이브러리 설정
target_link_libraries(PcdToCsv ${PCL_LIBRARIES} ${OpenCV_LIBS})
