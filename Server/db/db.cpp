#include "db.h"

// IMPORTANT: using YOUR local sqlite library
#include <sqlite3.h>

#include <iostream>

Database::Database(const std::string& db_path) {
    int rc = sqlite3_open(db_path.c_str(), &db);

    if (rc != SQLITE_OK) {
        std::cerr << "Database connection failed: "
                  << sqlite3_errmsg(db) << std::endl;
        db = nullptr;
    } else {
        std::cout << "Database connected successfully 🚀" << std::endl;
    }
}

Database::~Database() {
    if (db) {
        sqlite3_close(db);
    }
}

bool Database::execute(const std::string& query) {
    if (!db) {
        std::cerr << "DB is NULL (not connected)\n";
        return false;
    }

    char* errMsg = nullptr;

    int result = sqlite3_exec(db, query.c_str(), nullptr, nullptr, &errMsg);

    if (result != SQLITE_OK) {
        std::cerr << "SQL Error: " << errMsg << "\n";
        sqlite3_free(errMsg);
        return false;
    }

    return true;
}