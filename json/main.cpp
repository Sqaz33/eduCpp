#include <fstream>
#include <nlohmann/json.hpp>

int main() {
    std::ofstream f("example.json");

    nlohmann::json j2 = {
        {"pi", 3.141},
        {"happy", true},
        {"name", "Niels"},
        {"nothing", nullptr},
        {"answer", {
            {"everything", 42}
        }},
        {"list", {1, 0, 2}},
        {"object", {
            {"currency", "USD"},
            {"value", 42.99}
        }
        }
    };

    f << j2;
    f.close();

}