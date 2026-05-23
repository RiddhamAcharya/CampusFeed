#include "auth_routes.h"

#include <sqlite3.h>
#include <iostream>

void setupAuthRoutes(crow::SimpleApp& app, Database& database) {

    // Signup route
    CROW_ROUTE(app, "/signup")
    .methods("POST"_method)

    ([&database](const crow::request& req) {

        auto body = crow::json::load(req.body);

        if (!body) {
            return crow::response(400, "Invalid JSON");
        }

        std::string username = body["username"].s();
        std::string email = body["email"].s();
        std::string password = body["password"].s();

        // Validate fields
        if (username.empty() || email.empty() || password.empty()) {
            return crow::response(400, "All fields are required");
        }

        // Direct sqlite access
        sqlite3* db = database.db;

        const char* sql =
            "INSERT INTO users(username, email, password) "
            "VALUES(?, ?, ?);";

        sqlite3_stmt* stmt;

        int prepareResult =
            sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

        if (prepareResult != SQLITE_OK) {
            return crow::response(500, "Failed to prepare statement");
        }

        sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, email.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, password.c_str(), -1, SQLITE_TRANSIENT);

        int stepResult = sqlite3_step(stmt);

        sqlite3_finalize(stmt);

        if (stepResult != SQLITE_DONE) {
            return crow::response(500, "Signup failed");
        }

        return crow::response(200, "User created successfully");
    });
}