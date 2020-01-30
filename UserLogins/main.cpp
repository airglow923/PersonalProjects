#include <cstdio>

#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <sqlite3.h>

#include "./src/account.hpp"
#include "./src/database.hpp"
#include "./src/hashing.hpp"

int main()
{
    Database database;
    std::ofstream outFile("output");

    if (!!database.open_db_connection()) {
        std::cerr << "Failed to open.\n";
        exit(1);
    } else {
        std::cout << "Successfully opened.\n";
    }

    if (!!database.create_table_into_db()) {
        std::cerr << "Failed to create.\n";
        exit(1);
    } else {
        std::cout << "Successfully created.\n";
    }

    // 5e884898da28047151d0e56f8dc6292773603d0d6aabbdd62a11ef721d1542d8
    // if (!!database.insert_into_db("username", "password")) {
    //     std::cerr << "Failed to insert.\n";
    //     exit(1);
    // } else {
    //     std::cout << "Successfully inserted.\n";
    // }

    Table table = database.retrieve_db();

    if (!!database.close_db_connection()) {
        std::cerr << "Failed to close.\n";
        exit(1);
    } else {
        std::cout << "Successfully closed.\n";
    }

    for (std::size_t i = 0; i < table.size(); i++)
        for (std::size_t j = 0; j < table[0].size(); j++)
            std::cout << "results[" << i << "][" << j << "] = "
                      << table[i][j] << '\n';
}