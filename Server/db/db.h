#pragma once

#include <string>

// forward declare sqlite3 (from your local sqlite lib)
struct sqlite3;

class Database {
public:
    Database(const std::string& db_path);
    ~Database();

    bool execute(const std::string& query);

private:
    sqlite3* db;
};