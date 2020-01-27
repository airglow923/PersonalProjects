#include "database.hpp"

Database::Database()
    : m_administrator(Account("admin", "admin"))
{
    import("config.json");
}

Database::Database(
    const std::string& username,
    const std::string& password,
    const std::string& filename,
    const std::string& dbname)
    : m_administrator(Account(username, password)), m_dbname(dbname)
{
    import(filename);
    create_db();
}

bool Database::authenticate(
    const std::string& username,
    const std::string& password,
    const std::string& algorithm) const &
{
    Account access(username, password, algorithm);

    for (const auto& account : m_accounts)
        if (access == account)
            return true;
    return false;
}

void Database::add_user(
    const std::string& username,
    const std::string& password,
    const std::string& algorithm) &
{
    Account account(username, password, algorithm);

    if (!is_duplicate(account))
        m_accounts.push_back(account);
}

void Database::assign_admin(
    const std::string& username,
    const std::string& password,
    const std::string& algorithm) &
{
    std::string auth_user;
    std::string auth_pass;

    std::cout << "Enter administrator username: ";
    std::cin >> auth_user;
    std::cout << "Enter administrator password: ";
    std::cin >> auth_pass;

    if (m_administrator == Account(auth_user, auth_pass)) {
        Account account(username, password, algorithm);
        if (!is_duplicate(account))
            m_administrator = account;
    } else {
        std::cerr << "Access denied.\n";
    }
}

void Database::import(const std::string& filename)
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

    for (const auto& [key, value] : data.items()) {
        m_accounts.emplace_back(key, value, "PLAIN");
    }
}

void Database::save(const std::string& filename) const
{
    std::fstream outFile(filename, std::ios_base::out);
    json data = json::object();

    for (const auto& account : m_accounts)
        data.push_back({account.get_username(), account.get_hashed_pw()});

    outFile << std::setw(4) << data;
    outFile.close();
}

static int callback(void* NotUsed, int argc, char** argv, char** azColName)
{
    for (int i = 0; i < argc; i++)
        std::printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    std::putchar('\n');

    return 0;
}

int Database::execute_sql(const std::string& sql)
{
    char* zErrMsg = 0;

    open_db_connection();

    if (sqlite3_exec(m_db, sql.c_str(), callback, 0, &zErrMsg) != SQLITE_OK) {
        std::cerr << "Failed to execute SQL statements.\n";
        sqlite3_free(zErrMsg);
        return 1;
    } else {
        std::cout << "Successfully executed SQL statements.\n";
        return 0;
    }

    close_db_connection();
}

bool Database::is_duplicate(const Account& query)
{
    for (const auto& account : m_accounts)
        if (query.get_username() == account.get_username())
            return true;
    return false;
}

void Database::open_db_connection()
{
    if (sqlite3_open(("file:" + m_dbname).c_str(), &m_db) != SQLITE_OK) {
        std::cerr << "Failed to open database.\n";
        exit(EXIT_FAILURE);
    }
}

void Database::close_db_connection()
{
    if (sqlite3_close(m_db) != SQLITE_OK) {
        std::cerr << "Failed to close database.\n";
        exit(EXIT_FAILURE);
    }
}

void Database::create_db()
{
    open_db_connection();
    close_db_connection();
}

bool Database::startswith(const std::string& str, const std::string& prefix)
{
    for (std::size_t i = 0; i < prefix.size(); ++i)
        if (str[i] != prefix[i])
            return false;
    return true;
}

bool Database::endswith(const std::string& str, const std::string& prefix)
{
    for (int i = str.size() - 1, j = prefix.size() - 1;
        j >= 0;
        --i, --j)
        if (str[i] != prefix[j])
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