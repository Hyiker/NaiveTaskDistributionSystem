cmake_minimum_required(VERSION 3.16)
project(NaiveTaskDistributionSystem)

set(CMAKE_CXX_STANDARD 17)


include_directories(Client/src Server/src)

add_executable(Client Client/src/main.cpp)
add_executable(Server Server/src/main.cpp)