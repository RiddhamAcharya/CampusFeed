#include "notification_routes.h"
#include <sqlite3.h>
#include <iostream>

void setupNotificationRoutes(crow::SimpleApp& app, Database& database) {

    // fetch all notifications for a user
    CROW_ROUTE(app, "/notifications/<int>").methods("GET"_method)
    ([&database](int user_id) 
    {

        sqlite3* db = database.db;

        const char* sql =
            "SELECT id, title, message, is_read "
            "FROM notifications "
            "WHERE user_id = ?;";

        sqlite3_stmt* stmt;
        sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
        sqlite3_bind_int(stmt, 1, user_id);

        crow::json::wvalue result;
        int index = 0;

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            result[index]["id"]      = sqlite3_column_int(stmt, 0);
            result[index]["title"]   = (const char*)sqlite3_column_text(stmt, 1);
            result[index]["message"] = (const char*)sqlite3_column_text(stmt, 2);
            result[index]["is_read"] = sqlite3_column_int(stmt, 3);
            index++;
        }

        sqlite3_finalize(stmt);
        return crow::response(result);
    });

    // create a new notification
    CROW_ROUTE(app, "/notifications").methods("POST"_method)
    ([&database](const crow::request& req) {

        auto body = crow::json::load(req.body);

        if (!body) {
            return crow::response(400, "Invalid JSON");
        }

        int user_id         = body["user_id"].i();
        std::string title   = body["title"].s();
        std::string message = body["message"].s();

        if (title.empty() || message.empty()) {
            return crow::response(400, "title and message are required");
        }

        sqlite3* db = database.db;

        const char* sql =
            "INSERT INTO notifications (user_id, title, message) "
            "VALUES (?, ?, ?);";

        sqlite3_stmt* stmt;
        sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
        sqlite3_bind_int(stmt, 1, user_id);
        sqlite3_bind_text(stmt, 2, title.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, message.c_str(), -1, SQLITE_TRANSIENT);

        int stepResult = sqlite3_step(stmt);
        sqlite3_finalize(stmt);

        if (stepResult != SQLITE_DONE) {
            return crow::response(500, "Failed to create notification");
        }

        return crow::response(200, "Notification created");
    });

    // mark a notification as read
    CROW_ROUTE(app, "/notifications/<int>/read").methods("PATCH"_method)
    ([&database](int id)
    {

        sqlite3* db = database.db;

        const char* sql =
            "UPDATE notifications SET is_read = 1 WHERE id = ?;";

        sqlite3_stmt* stmt;
        sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
        sqlite3_bind_int(stmt, 1, id);

        int stepResult = sqlite3_step(stmt);
        sqlite3_finalize(stmt);

        if (stepResult != SQLITE_DONE) {
            return crow::response(500, "Failed to mark as read");
        }

        return crow::response(200, "Notification marked as read");
    });
}