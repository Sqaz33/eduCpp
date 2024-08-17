/*
    libpqxx - библиотека, работающая поверх API библиотеки libpq.
    
    Кодирование с этой библиотекой - использование транзакций.
    
    Типы транзакций:
        * Транзакционные.
        * Нетранзакционные (автоматическая фиксация).
    
    Виды запросов 
        * query - получение всех данных, преобразованных в нужные типы данных.
        * stream - поток по запросу, дольше выполнение метода, но быстрый последовательный доступ к данным.
        * exec - получение результата в виде объекта с метаданными.
*/
#include <exception>
#include <iostream>

#include <pqxx/pqxx>

#include "windows.h"

int main() {
    SetConsoleOutputCP(CP_UTF8); 
    
    // объект подключение к базе данныех
    pqxx::connection con("dbname=cpptest user=postgres password=1568");

    // алиас к transaction<> 
    // объект транзакции
    pqxx::work txn(con);
    

    try {      
        // фул данные
        for (auto [numb, salary] : 
            txn.query<int, int>("select numb, salary from Employee")) {
            std::cout << numb << "->" << salary << '\n';
        }

        // данные по мере поступления
        for (auto [numb, salary] : 
            txn.stream<int, int>("select numb, salary from Employee")) {
            std::cout << numb << "->" << salary << '\n';
        }

        // сырые данные
        auto r = txn.exec("select numb, salary from Employee");
        for (const auto& row : r) {
            for (const auto& field : row) {
                std::cout << field.c_str() << '\n';
            }
        }

    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }


}
