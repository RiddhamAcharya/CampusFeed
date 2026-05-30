#include "user_routes.h"
#include "../middleware/auth_middleware.h"
#include <sqlite3.h>

void setupUserRoutes(App& app, Database& database)
{
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
}