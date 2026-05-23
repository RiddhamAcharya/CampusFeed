#pragma once

#include <string>
#include <sqlite3.h>

class Database {
public:

    sqlite3* db;

    Database(const std::string& db_path);

    ~Database();

    bool execute(const std::string& query);
};
