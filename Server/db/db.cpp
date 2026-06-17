#include "db.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <sqlite3.h>
#include <iostream>

Database::Database(const std::string& db_path) {
    int rc = sqlite3_open(db_path.c_str(), &db);

    if (rc != SQLITE_OK) {
        std::cerr << "Database connection failed: "
                  << sqlite3_errmsg(db) << std::endl;
        db = nullptr;
    } else {
        std::cout << "Database connected successfully" << std::endl;
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
void Database::loadAllSchemas(const std::string& folderPath) {

    //Load Schemas files
    
    std::vector<std::string> files = {

        folderPath + "/users.sql",
        folderPath + "/events.sql",
        folderPath + "/notifications.sql",
        folderPath + "/interactions.sql",
        folderPath + "/indexes.sql"
    };

    for (const auto& path : files) {

        std::ifstream file(path);

        if (!file.is_open()) {
            std::cerr << "Failed to open: " << path << std::endl;
            continue;
        }

        std::stringstream buffer;
        buffer << file.rdbuf();

        std::string sql = buffer.str();

        char* errMsg = nullptr;

        int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);

        if (rc != SQLITE_OK) {
            std::cerr << "Schema error in " << path
                      << " : " << errMsg << std::endl;

            sqlite3_free(errMsg);
        } else {
            std::cout << "Loaded schema: " << path << std::endl;
        }
    }
}