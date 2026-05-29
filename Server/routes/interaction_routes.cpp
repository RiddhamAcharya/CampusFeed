#include "interaction_routes.h"
#include <sqlite3.h>
#include <iostream>
void setupInteractionRoutes(App& app, Database& database) {
    CROW_ROUTE(app, "/interactions").methods("POST"_method)
([&database](const crow::request& req) {

    auto body = crow::json::load(req.body);

    if (!body) {
        return crow::response(400, "Invalid JSON");
    }

    int user_id             = body["user_id"].i();
    int event_id            = body["event_id"].i();
    std::string interaction = body["interaction_type"].s();

    if (interaction != "interested" &&
        interaction != "going" &&
        interaction != "not_interested") {
        return crow::response(400, "Invalid interaction type");
    }

    sqlite3* db = database.db;

    const char* sql =
        "INSERT OR REPLACE INTO event_interactions "
        "(user_id, event_id, interaction_type) "
        "VALUES (?, ?, ?);";

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    sqlite3_bind_int(stmt, 1, user_id);
    sqlite3_bind_int(stmt, 2, event_id);
    sqlite3_bind_text(stmt, 3, interaction.c_str(), -1, SQLITE_TRANSIENT);

    int result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (result != SQLITE_DONE) {
        return crow::response(500, "Failed to save interaction");
    }

    return crow::response(200, "Interaction saved");
});
CROW_ROUTE(app, "/interactions/<int>/<int>").methods("GET"_method)
([&database](int event_id, int user_id) {

    sqlite3* db = database.db;

    const char* sql =
        "SELECT interaction_type "
        "FROM event_interactions "
        "WHERE user_id = ? AND event_id = ?;";

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    sqlite3_bind_int(stmt, 1, user_id);
    sqlite3_bind_int(stmt, 2, event_id);

    int rc = sqlite3_step(stmt);

    crow::json::wvalue result;

    if (rc == SQLITE_ROW) {
        result["interaction_type"] =
            std::string((const char*)sqlite3_column_text(stmt, 0));
    } else {
        result["interaction_type"] = "none";
    }

    sqlite3_finalize(stmt);
    return crow::response(result);
});
CROW_ROUTE(app, "/interactions/<int>/count").methods("GET"_method)
([&database](int event_id) {

    sqlite3* db = database.db;

    const char* sql =
        "SELECT interaction_type, COUNT(*) as total "
        "FROM event_interactions "
        "WHERE event_id = ? "
        "GROUP BY interaction_type;";

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    sqlite3_bind_int(stmt, 1, event_id);

    crow::json::wvalue result;
    result["interested"]     = 0;
    result["going"]          = 0;
    result["not_interested"] = 0;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string type =
            (const char*)sqlite3_column_text(stmt, 0);
        int count        =
            sqlite3_column_int(stmt, 1);
        result[type]     = count;
    }

    sqlite3_finalize(stmt);
    return crow::response(result);
});
}