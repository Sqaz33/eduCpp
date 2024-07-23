find_package(Crow CONFIG REQUIRED)
add_executable(crow main.cpp)
target_link_libraries(crow PRIVATE Crow::Crow asio::asio)