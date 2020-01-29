#include <cstdio>

#include <iostream>
#include <string>
#include <utility>
#include <sqlite3.h>

#include "./src/account.hpp"
#include "./src/database.hpp"
#include "./src/hashing.hpp"

static int callback(void* NotUsed, int argc, char** argv, char** azColName)
{
    for (int i = 0; i < argc; i++)
        std::printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    std::putchar('\n');

    return 0;
}

int main()
{
    Database database;

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

    // 2cf24dba5fb0a30e26e83b2ac5b9e29e1b161e5c1fa7425e73043362938b9824
    Account account("anon", "hello");
    if (!!database.insert_into_db(account)) {
        std::cerr << "Failed to insert.\n";
        exit(1);
    } else {
        std::cout << "Successfully inserted.\n";
    }
    
    // if (!!database.display_db()) {
    //     std::cerr << "Failed to display.\n";
    //     exit(1);
    // }

    if (!!database.close_db_connection()) {
        std::cerr << "Failed to close.\n";
        exit(1);
    } else {
        std::cout << "Successfully closed.\n";
    }

    // database.add_user("poopity", "scoopity");
    // database.add_user("username", "password");
    // database.add_user("u$ern@me", "p@$$w0rd");
    // database.add_user("sample1", "somepass");

    // database.save("config.json");

    // sqlite3* db;
    // char* zErrMsg = 0;
    // std::string sql1 =
    //     "CREATE TABLE IF NOT EXISTS account(" \
    //         "username           VARCHAR(" +
    //         std::to_string(USERNAME_LEN) + ") NOT NULL," \
    //         "hashed_password    CHAR(" +
    //         std::to_string(HASHED_PW_LEN) + ") NOT NULL);";
    // std::string sql2 =
    //     "INSERT INTO account " \
    //     "VALUES ('" \
    //     "anon', '" \
    //     "hello');";

    // if (sqlite3_open("file:account.db", &db) != SQLITE_OK) {
    //     std::cerr << "Failed to open database.\n";
    //     exit(EXIT_FAILURE);
    // } else {
    //     std::cout << "Successfully opened database.\n";
    // }

    // if (sqlite3_exec(db, sql1.c_str(), callback, 0, &zErrMsg) != SQLITE_OK) {
    //     if (zErrMsg) {
    //         std::cerr << "SQL error: " << zErrMsg << '\n';
    //         sqlite3_free(zErrMsg);
    //     }
    //     return 1;
    // } else {
    //     std::cout << "Successfully executed SQL statements.\n";
    // }

    // if (sqlite3_exec(db, sql2.c_str(), callback, 0, &zErrMsg) != SQLITE_OK) {
    //     if (zErrMsg) {
    //         std::cerr << "SQL error: " << zErrMsg << '\n';
    //         sqlite3_free(zErrMsg);
    //     }
    //     return 1;
    // } else {
    //     std::cout << "Successfully executed SQL statements.\n";
    // }

    // if (sqlite3_close(db) != SQLITE_OK) {
    //     std::cerr << "Failed to close database.\n";
    //     exit(EXIT_FAILURE);
    // } else {
    //     std::cout << "Successfully closed database.\n";
    // }

    // sqlite3* db;

    // if (!!Database::open_db_connection("account.db", db)) {
    //     std::cerr << "Failed to open.\n";
    //     exit(1);
    // }

    // if (!!Database::create_table_into_db(db, "account")) {
    //     std::cerr << "Failed to create.\n";
    //     exit(1);
    // }

    // if (!!Database::insert_into_db(db, "anon", "hello")) {
    //     std::cerr << "Failed to insert.\n";
    //     exit(1);
    // }
    
    // if (!!Database::close_db_connection(db)) {
    //     std::cerr << "Failed to close.\n";
    //     exit(1);
    // }
}