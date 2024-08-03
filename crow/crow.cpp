
// Приложение
/*
    Приложение Crow определяет интерфейс, 
    позволяющий разработчику получать доступ ко всем 
    различным частям фреймворка без 
    необходимости вручную разбираться с каждой из них.
*/
int main() {
    // App with middleware
    crow::App<middleware ...> app;
    // Simple app 
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([]() {
        return "Hello world";
    });

    app.bindaddr("192.168.0.102")
    .port(8000)
    .multithreaded()
    .run();

}


//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
// Маршруты
/*
    Маршруты определяют, что происходит, 
    когда ваш клиент подключается к определенному URL.


    <схема>:[//[<логин>[:<пароль>]@]<хост>[:<порт>]][/<URL‐путь>][?<параметры>][#<якорь>]
*/

struct MyInt : public crow::returnable {
    MyInt(int numb) : 
        numb(numb),
        returnable("text/plain") 
    {}

    std::string dump() const override {
        return std::to_string(numb);
    }
private:
    int numb;
    
};

int main() {

    crow::SimpleApp app;
    // Макрос определения маршрута
    CROW_ROUTE(app, "/") (
        // Обработчик, выполняется, когда клиент вызывает связанный маршрут 
        []() {
            return "Hello!";
        }
    );

    // Использование параметров в маршруте
    CROW_ROUTE(app, "/add/<int>/<int>") (
        [](int a, int b) {
            return std::to_string(a + b);
        }
    );

    // HTTP-методы в макросах определения маршрута - method()
    CROW_ROUTE(app, "/hello").methods(crow::HTTPMethod::GET) (
        []() {
            return "Hi";
        }
    );

    // Обработка параметров URL
    CROW_ROUTE(app, "/search") (
        [](const crow::request& req) {
            return req.url_params.get("q");
        }
    );

    // Возвращение HTTP-ответа
    CROW_ROUTE(app, "/check") (
        []() {
            // crow::response resp(200); код ответа числом 
            crow::response resp(crow::status::OK); 
            resp.write("1234");
            return resp;
        }
    );

    // Возвращение пользовательских классов
    CROW_ROUTE(app, "/class") (
        []() {
            MyInt n(1);
            return n;
        }
    );

    // Общие маршруты
    CROW_CATCHALL_ROUTE(app) (
        []() {
            return "unknow URL";
        }
    );


    app.bindaddr("192.168.0.102")
    .port(8000)
    .run();


}


//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
class CustomLogger : public crow::ILogHandler {
public:
    CustomLogger() = default;
    void log(std::string message, crow::LogLevel lvl) {
        std::cerr << message << std::endl;
    }
};

// Логирование
int main() {


    CustomLogger logger;
    // замещене стандартного логгера
    crow::logger::setHandler(&logger);

    crow::SimpleApp app;

    // Настройка уровня логирования приложения
    app.loglevel(crow::LogLevel::Info);
    


    //само логирования 
    CROW_ROUTE(app, "/")([]() {
        CROW_LOG_INFO << "info log";
        CROW_LOG_CRITICAL << "critical log";
        return "hello";
    });

    app.bindaddr("192.168.0.102")
    .port(8000)
    .multithreaded()
    .run();
}

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//mustache templates
int main() {
    // установить путь до каталога templetes
    crow::mustache::set_base("D:/eduCpp/crow/templates");

            
    //загрузить страницу mustache
    // путь указывается относительно templates директории
    auto rootPage = crow::mustache::load("root.html");
    
    // или вычислить из строки
    auto compPage = crow::mustache::compile("hi {{value}}");

    // контекс 
    crow::mustache::context ctx;

    ctx["title"] = "Hello page";
    ctx["heading"] = "привет";
    ctx["message"] = "2";

    crow::SimpleApp app;
    CROW_ROUTE(app, "/")([&rootPage, &ctx]() {
        auto response = crow::response(std::move(rootPage.render(ctx)));
        response.set_header("Content-Type", "text/html; charset=UTF-8");
        return response;
    });

    CROW_CATCHALL_ROUTE(app) ([&compPage]() {
        crow::mustache::context ctx;
        ctx["value"] = "неизвестный url";
        return compPage.render(ctx);
    });

    app.bindaddr("192.168.0.102")
    .port(8000)
    .multithreaded()
    .run();
}

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
// строки запроса 
// http://example.com?key=value&otherkey=othervalue
// http://example.com?key[]=value1&key[]=value2&key[]=value - list
// key[sub_key1]=value1&key[sub_key2]=value2&key[sub_key3]=value3 - dict
int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app,  "/")([](const crow::request& req){
        const auto& params = req.url_params;
        char* c = params.get("q");
        return c == nullptr ? "unknow url" : c;
    });
    
    
    CROW_ROUTE(app,  "/list")([](const crow::request& req){
        const auto& params = req.url_params;
        std::vector<char*> parList = params.get_list("name");

        std::string ret;
        if (parList.size()) {
            for (const auto& p : parList) {
                ret += p;
                ret += "\n";
            }   
        } else  {
            return std::string("unknow url");
        }
        return ret;
    });
    
    CROW_ROUTE(app, "/dict")([](const crow::request& req) {
        const auto& params = req.url_params;
        std::unordered_map<std::string, std::string> parDict = params.get_dict("name");
        
        std::string ret;
        if (parDict.size()) {
            for (auto& p : parDict) {
                ret += p.first + " -> " + p.second;
                ret += "\n";
            }
        } else {
            return std::string("unknow url");
        }
        return ret;
    });
    
    app.bindaddr("192.168.0.102")
    .port(8000)
    .multithreaded()
    .run();
}

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------