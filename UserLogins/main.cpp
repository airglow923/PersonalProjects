#include <cstdio>

#include <iostream>
#include <string>
#include <utility>
#include <sqlite3.h>

#include "./src/account.hpp"
#include "./src/database.hpp"
#include "./src/hashing.hpp"

int main()
{
    Database database;

    database.add_user("user1", "hash");
    database.add_user("user2", "password");
    database.add_user("user3", "p@$$w0rd");
    database.add_user("user4", "anon", "MD5");
    database.add_user("user5", "random");
    database.add_user("user6", "test", "MD5");

    if (!!database.open_db_connection()) {
        std::cerr << "Failed to open.\n";
        exit(1);
    } else {
        std::cout << "Successfully opened.\n";
    }

    if (!!database.update_db()) {
        std::cerr << "Failed to update.\n";
        exit(1);
    } else {
        std::cout << "Successfully updated.\n";
    }

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