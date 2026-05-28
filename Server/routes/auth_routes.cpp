#include "auth_routes.h"
#include "../db/db.h"
#include "../utils/hash.h"
#include "../utils/jwt.h"
#include "../utils/app.h"

#include <sqlite3.h>
#include <iostream>

void setupAuthRoutes(App& app, Database& database) {
    
    // SIGNUP ROUTE
    
    CROW_ROUTE(app, "/signup").methods("POST"_method)
    ([&database](const crow::request& req) 
    {

        auto body = crow::json::load(req.body);

        if (!body) {
            return crow::response(400, "Invalid JSON");
        }

        
        std::string full_name = std::string(body["full_name"].s());
        std::string email = std::string(body["email"].s());
        std::string password = std::string(body["password"].s());
        std::string role = std::string(body["role"].s());

        std::string institution = body.has("institution")
            ? std::string(body["institution"].s())
            : "";

        std::string profile_image = body.has("profile_image")
            ? std::string(body["profile_image"].s())
            : "";

        // Basic validation
        if (full_name.empty() || email.empty() || password.empty() || role.empty()) {
            return crow::response(400, "Missing required fields");
        }

        // Role validation (matches your schema constraint)
        if (role != "student" && role != "organizer" && role != "admin") {
            return crow::response(400, "Invalid role");
        }

        // Hash password using openssl
        std::string password_hash = hashPassword(password);

        sqlite3* db = database.db;

        const char* sql =
            "INSERT INTO users (full_name, email, password_hash, role, institution, profile_image) "
            "VALUES (?, ?, ?, ?, ?, ?)";

        sqlite3_stmt* stmt;

        int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

        if (rc != SQLITE_OK) {
            return crow::response(500, "Failed to prepare statement");
        }

        sqlite3_bind_text(stmt, 1, full_name.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, email.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, password_hash.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 4, role.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 5, institution.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 6, profile_image.c_str(), -1, SQLITE_TRANSIENT);

        rc = sqlite3_step(stmt);

        sqlite3_finalize(stmt);

        if (rc != SQLITE_DONE) {
            return crow::response(500, "Signup failed (email may already exist)");
        }

        return crow::response(200, "User created successfully");
    });

    // LOGIN ROUTE

    CROW_ROUTE(app, "/login").methods("POST"_method)
    ([&database](const crow::request& req)
    {

        auto body = crow::json::load(req.body);

        if (!body) {
            return crow::response(400, "Invalid JSON");
        }

        std::string email = std::string(body["email"].s());
        std::string password = std::string(body["password"].s());

        if (email.empty() || password.empty()) {
            return crow::response(400, "Email and password required");
        }

        sqlite3* db = database.db;

        const char* sql =
            "SELECT id, full_name, email, password_hash, role "
            "FROM users WHERE email = ?";

        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
            return crow::response(500, "DB error");
        }

        sqlite3_bind_text(stmt, 1, email.c_str(), -1, SQLITE_TRANSIENT);

        int rc = sqlite3_step(stmt);

        if (rc != SQLITE_ROW) {
            sqlite3_finalize(stmt);
            return crow::response(401, "User not found");
        }

        int id = sqlite3_column_int(stmt, 0);

        std::string full_name = (const char*)sqlite3_column_text(stmt, 1);
        std::string db_email = (const char*)sqlite3_column_text(stmt, 2);
        std::string db_password_hash = (const char*)sqlite3_column_text(stmt, 3);
        std::string role = (const char*)sqlite3_column_text(stmt, 4);

        sqlite3_finalize(stmt);

        // Hash incoming password
        std::string input_hash = hashPassword(password);

        if (input_hash != db_password_hash) {
            return crow::response(401, "Invalid password");
        }

        std::string token = create_token(id);

        crow::json::wvalue res;

        res["message"] = "Login successful";
        res["token"] = token;

        res["user"]["id"] = id;
        res["user"]["full_name"] = full_name;
        res["user"]["email"] = db_email;
        res["user"]["role"] = role;

        return crow::response(res);
    });
}