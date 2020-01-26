#include <iostream>
#include <string>

#include "./src/database.hpp"
#include "./src/hashing.hpp"

int main()
{
    Database database;

    database.add_user("poopity", "scoopity");

    if (database.authenticate("poopity", "scoopity"))
        std::cout << "Authenticated\n";
}