CMAKE_MINIMUM_REQUIRED(VERSION 2.8.0)
PROJECT(bintoheader)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)

add_executable(bintoheader bintoheader.cpp)
install(TARGETS bintoheader DESTINATION bin)
