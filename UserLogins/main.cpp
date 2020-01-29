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

    // 2cf24dba5fb0a30e26e83b2ac5b9e29e1b161e5c1fa7425e73043362938b9824
    database.insert_into_db("anon", "hello");
    database.display_db();

    // database.add_user("poopity", "scoopity");
    // database.add_user("username", "password");
    // database.add_user("u$ern@me", "p@$$w0rd");
    // database.add_user("sample1", "somepass");

    // database.save("config.json");
}