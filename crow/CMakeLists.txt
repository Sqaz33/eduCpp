find_package(Crow CONFIG REQUIRED)
# find_package(Boost REQUIRED COMPONENTS log)

add_executable(crow main.cpp)

target_link_libraries(crow PRIVATE Crow::Crow asio::asio)
# target_link_libraries(crow PRIVATE Boost::log)
