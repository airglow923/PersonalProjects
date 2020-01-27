#include <iostream>
#include <string>

#include "./src/database.hpp"
#include "./src/hashing.hpp"

int main()
{
    Database database;

    database.add_user("poopity", "scoopity");
    database.add_user("username", "password");
    database.add_user("u$ern@me", "p@$$w0rd");
    database.add_user("sample1", "somepass");
    database.assign_admin("new", "admin");

    database.save("config.json");
}