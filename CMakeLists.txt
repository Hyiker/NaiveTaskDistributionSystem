cmake_minimum_required(VERSION 3.16)
project(NaiveTaskDistributionSystem)

set(CMAKE_CXX_STANDARD 17)


include_directories(Client/src Server/src /usr/local/include shared)

set(SHARED_LIBS shared/task.hpp shared/file.hpp)

add_executable(Client Client/src/main.cpp ${SHARED_LIBS} shared/utils.hpp Client/src/client.hpp)
add_executable(Server Server/src/main.cpp ${SHARED_LIBS} Server/src/server.hpp)