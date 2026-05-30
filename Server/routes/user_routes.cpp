#include "user_routes.h"
#include "../middleware/auth_middleware.h"
#include <sqlite3.h>

void setupUserRoutes(App& app, Database& database)
{
    // GET /profile to retrieve user's profile information
    
    CROW_ROUTE(app, "/profile").methods("GET"_method)
    ([&app, &database](const crow::request& req)
    {
        auto& ctx = app.template get_context<AuthMiddleware>(req);

        int user_id = ctx.user_id;

        if (user_id == -1)
        {
            return crow::response(401, "Unauthorized");
        }

        sqlite3* db = database.db;

        const char* sql =
            "SELECT id, full_name, email, role, institution "
            "FROM users WHERE id = ?";

        sqlite3_stmt* stmt;

        sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
        sqlite3_bind_int(stmt, 1, user_id);

        int rc = sqlite3_step(stmt);

        if (rc != SQLITE_ROW)
        {
            sqlite3_finalize(stmt);
            return crow::response(404, "User not found");
        }

        crow::json::wvalue res;

        res["id"] = sqlite3_column_int(stmt, 0);
        res["full_name"] = (const char*)sqlite3_column_text(stmt, 1);
        res["email"] = (const char*)sqlite3_column_text(stmt, 2);
        res["role"] = (const char*)sqlite3_column_text(stmt, 3);
        res["institution"] = (const char*)sqlite3_column_text(stmt, 4);

        sqlite3_finalize(stmt);

        return crow::response(res);
    });

    //PUT /profile to update user's full name and institution

    CROW_ROUTE(app, "/profile").methods("PUT"_method)
    ([&app, &database](const crow::request& req)
    {
        auto& ctx = app.template get_context<AuthMiddleware>(req);

        int user_id = ctx.user_id;

        if (user_id == -1)
        {
            return crow::response(401, "Unauthorized");
        }

        auto body = crow::json::load(req.body);

        if (!body)
        {
            return crow::response(400, "Invalid JSON");
        }

        std::string full_name = body.has("full_name")
            ? std::string(body["full_name"].s())
            : "";

        std::string institution = body.has("institution")
            ? std::string(body["institution"].s())
            : "";

        if (full_name.empty() && institution.empty())
        {
            return crow::response(400, "Nothing to update");
        }

        sqlite3* db = database.db;

        const char* sql =
            "UPDATE users SET full_name = COALESCE(NULLIF(?, ''), full_name), "
            "institution = COALESCE(NULLIF(?, ''), institution) "
            "WHERE id = ?";

        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        {
            return crow::response(500, "DB error");
        }

        sqlite3_bind_text(stmt, 1, full_name.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, institution.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 3, user_id);

        int rc = sqlite3_step(stmt);

        sqlite3_finalize(stmt);

        if (rc != SQLITE_DONE)
        {
            return crow::response(500, "Update failed");
        }

        return crow::response(200, "Profile updated successfully");
    });
}