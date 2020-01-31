#include "database.hpp"

Database::Database()
    : m_administrator(Account("admin", "admin")),
        m_dbname("account.db"), m_db(nullptr)
{
    if (db_exists()) {
        import_from_sql();
    } else {
        open_db_connection();
        create_table_into_db();
        close_db_connection();
    }

}

Database::Database(
    const std::string& username,
    const std::string& password,
    const std::string& filename)
    : m_administrator(Account(username, password))
{
    if (endswith(filename, "json")) {
        import_json(filename);
    } else {
        if (db_exists()) {
            import_from_sql();
        } else {
            open_db_connection();
            create_table_into_db();
            close_db_connection();
        }
    }
}

void Database::import_json(const std::string& filename)
{
    std::fstream inFile(filename, std::ios_base::in);

    if (!inFile)
        return;

    if (inFile.peek() == std::ifstream::traits_type::eof()) {
        inFile.close();
        return;
    }

    std::string content(
        (std::istreambuf_iterator<char>(inFile)),
        std::istreambuf_iterator<char>());

    inFile.close();

    json data = json::parse(content.c_str());

    for (const auto& [key, value] : data.items())
        m_accounts.emplace_back(key, value, "PLAIN");
}

void Database::export_as_json(const std::string& filename) const
{
    std::fstream outFile(filename, std::ios_base::out);
    json data = json::object();

    for (const auto& account : m_accounts)
        data.push_back({account.get_username(), account.get_hashed_pw()});

    outFile << std::setw(4) << data;
    outFile.close();
}

static int print_callback(
    void* NotUsed, int argc, char** argv, char** colName)
{
    for (int i = 0; i < argc; i++)
        std::printf("%s = %s\n", colName[i], argv[i] ? argv[i] : "NULL");
    std::putchar('\n');

    return 0;
}

static int retrieve_callback(
    void* dest, int argc, char** argv, char** colName)
{
    Table* table = static_cast<Table*>(dest);

    try {
        table->emplace_back(argv, argv + argc);
    } catch (...) {
        return 1;
    }

    return 0;
}

void Database::import_from_sql()
{
    open_db_connection();
    Table table = retrieve_db();
    close_db_connection();

    for (const auto& row : table)
        m_accounts.emplace_back(row[0], row[1], row[2]);
}

int Database::open_db_connection()
{
    int res = sqlite3_open(("file:" + m_dbname).c_str(), &m_db);
    
    if (res != SQLITE_OK) {
        std::cerr << "Failed to open database.\n";
        return res;
    }

    return SQLITE_OK;
}

int Database::open_db_connection(const std::string& dbname, sqlite3* db)
{
    int res = sqlite3_open(("file:" + dbname).c_str(), &db);
    
    if (res != SQLITE_OK) {
        std::cerr << "Failed to open database.\n";
        return res;
    }

    return SQLITE_OK;
}

int Database::close_db_connection()
{
    int res = sqlite3_close(m_db);

    if (res != SQLITE_OK) {
        std::cerr << "Failed to close database.\n";
        return res;
    }
    
    return 0;
}

int Database::close_db_connection(sqlite3* db)
{
    int res = sqlite3_close(db);

    if (res != SQLITE_OK) {
        std::cerr << "Failed to close database.\n";
        return res;
    }
    
    return 0;
}

int Database::execute_sql(
    const std::string& sql,
    int(*callback)(void*, int, char**, char**),
    Table* table)
{
    char* zErrMsg = 0;
    int res = sqlite3_exec(m_db, sql.c_str(), callback, table, &zErrMsg);

    if (res != SQLITE_OK) {
        if (zErrMsg) {
            std::cerr << "SQL error: " << zErrMsg << '\n';
            sqlite3_free(zErrMsg);
        }
        return res;
    }

    return 0;
}

int Database::execute_sql(
    sqlite3* db,
    const std::string& sql,
    int(*callback)(void*, int, char**, char**),
    Table* table)
{
    char* zErrMsg = 0;
    int res = sqlite3_exec(db, sql.c_str(), callback, table, &zErrMsg);

    if (res != SQLITE_OK) {
        if (zErrMsg) {
            std::cerr << "SQL error: " << zErrMsg << '\n';
            sqlite3_free(zErrMsg);
        }
        return res;
    }

    return 0;
}

int Database::update_db()
{
    for (const auto& account : m_accounts)
        if (insert_into_db(account) != SQLITE_OK)
            return 1;
        
    return 0;
}

int Database::insert_into_db(const Account& account)
{
    std::string sql =
        "INSERT OR IGNORE INTO account " \
        "VALUES ('" +
        account.get_username()  + "', '" +
        account.get_hashed_pw() + "', '" +
        account.get_algorithm() + "');";

    return execute_sql(sql, print_callback);
}

int Database::insert_into_db(Account&& account)
{
    return insert_into_db(static_cast<const Account&>(std::move(account)));
}

int Database::insert_into_db(sqlite3* db, const Account& account)
{
    std::string sql =
        "INSERT OR IGNORE INTO account " \
        "VALUES ('" +
        account.get_username()  + "', '" +
        account.get_hashed_pw() + "', '" +
        account.get_algorithm() + "');";

    return execute_sql(db, sql, print_callback);
}

int Database::insert_into_db(sqlite3* db, Account&& account)
{
    return insert_into_db(db, static_cast<const Account&>(std::move(account)));
}

int Database::display_db()
{
    std::string sql =
        "SELECT * FROM account;";

    return execute_sql(sql, print_callback);
}

int Database::display_db(sqlite3* db)
{
    std::string sql =
        "SELECT * FROM account;";

    return execute_sql(db, sql, print_callback);
}

int Database::delete_db()
{
    std::string sql =
        "DROP TABLE account;";

    return execute_sql(sql, print_callback);
}

int Database::delete_db(sqlite3* db)
{
    std::string sql =
        "DROP TABLE account;";

    return execute_sql(db, sql, print_callback);
}

Table Database::retrieve_db()
{
    std::string sql =
        "SELECT * FROM account;";
    Table table;

    if (!!execute_sql(sql, retrieve_callback, &table)) {
        std::cerr << "Failed to retrieve.\n";
        exit(EXIT_FAILURE);
    }
    
    return table;
}

Table Database::retrieve_db(sqlite3* db)
{
    std::string sql =
        "SELECT * FROM account;";
    Table table;

    if (!!execute_sql(db, sql, retrieve_callback, &table)) {
        std::cerr << "Failed to retrieve.\n";
        exit(EXIT_FAILURE);
    }
    
    return table;
}

bool Database::is_duplicate(const Account& query)
{
    for (const auto& account : m_accounts)
        if (query.get_username() == account.get_username())
            return true;
    return false;
}

bool Database::db_exists() const
{
    return fs::exists(fs::current_path() / m_dbname);
}

int Database::create_table_into_db()
{
    std::string sql =
        "CREATE TABLE IF NOT EXISTS account(" \
            "username           VARCHAR(" +
            std::to_string(NAME_MAX_LEN) + ") NOT NULL PRIMARY KEY," \
            "hashed_password    VARCHAR(" +
            std::to_string(HASH_MAX_LEN) + ") NOT NULL," \
            "algorithm          VARCHAR(" +
            std::to_string(ALGO_MAX_LEN) + ") NOT NULL);";

    return execute_sql(sql, print_callback);
}

int Database::create_table_into_db(sqlite3* db, const std::string& table)
{
    std::string sql =
        "CREATE TABLE IF NOT EXISTS " + table + "(" \
            "username           VARCHAR(" +
            std::to_string(NAME_MAX_LEN) + ") NOT NULL PRIMARY KEY," \
            "hashed_password    VARCHAR(" +
            std::to_string(HASH_MAX_LEN) + ") NOT NULL," \
            "algorithm          VARCHAR(" +
            std::to_string(ALGO_MAX_LEN) + ") NOT NULL);";

    return execute_sql(db, sql, print_callback);
}

bool Database::startswith(const std::string& str, const std::string& prefix)
{
    for (std::size_t i = 0; i < prefix.size(); ++i)
        if (str[i] != prefix[i])
            return false;
    return true;
}

bool Database::endswith(const std::string& str, const std::string& suffix)
{
    for (int i = str.size() - 1, j = suffix.size() - 1;
        j >= 0;
        --i, --j)
        if (str[i] != suffix[j])
            return false;
    return true;
}

std::string Database::get_db_name(
    const std::string& haystack,
    const std::string& needle,
    const std::string& option)
{
    if (option == "p")
        return startswith(haystack, needle) ? haystack : needle + haystack;
    else if (option == "s")
        return endswith(haystack, needle) ? haystack : haystack + needle;
    return nullptr;
}