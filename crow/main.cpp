#include <crow.h>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>


// Функция для чтения файлов
std::string read_file(const std::string& file_path) {
    std::ifstream file(file_path);
    std::stringstream buffer;
    buffer << file.rdbuf(); 
    return buffer.str();
}

// Функция для чтения бинарных файлов
std::vector<uint8_t> read_binary_file(const std::string& file_path) {
    std::ifstream file(file_path, std::ios::binary);
    return std::vector<uint8_t>((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

int main() {
    crow::SimpleApp app;

    // Устанавливаем путь к шаблонам mustache
    crow::mustache::set_global_base("D:/eduCpp/crow/static/templates");

    CROW_ROUTE(app, "/")([]() {
        auto page = crow::mustache::load("index.html");
        return page.render();
    });


    // Маршрут для CSS-файлов
    CROW_ROUTE(app, "/css/<string>")([](const std::string& filename){
        return read_file("D:/eduCpp/crow/static/css/" + filename);
    });

    // Маршрут для JavaScript-файлов
    CROW_ROUTE(app, "/js/<string>")([](const std::string& filename){
        return read_file("D:/eduCpp/crow/static/js/" + filename);
    });

    app.bindaddr("192.168.0.102")
    .port(8000)
    .multithreaded()
    .run();
    return 0;
}