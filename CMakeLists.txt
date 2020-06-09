cmake_minimum_required(VERSION 3.16)
project(NaiveTaskDistributionSystem)

set(CMAKE_CXX_STANDARD 17)
include_directories(Client/src Server/src /usr/local/include shared ${OPENSSL_INCLUDE_DIRS})
link_directories(/usr/local/lib)
link_libraries(ssl crypto)


set(SHARED_LIBS shared/task.hpp shared/file.hpp shared/serializable.hpp shared/request.hpp shared/invalid_data_stream.hpp shared/invalid_file.hpp shared/task_manager.hpp shared/file_manager.hpp)

add_executable(Client Client/src/main.cpp ${SHARED_LIBS} shared/utils.hpp Client/src/client.hpp)
add_executable(Server Server/src/main.cpp ${SHARED_LIBS} Server/src/server.hpp)
target_link_libraries(Client ssl)
target_link_libraries(Server ssl)