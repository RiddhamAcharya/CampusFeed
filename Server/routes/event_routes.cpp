#include "event_routes.h"

#include <sqlite3.h>

void setupEventRoutes(App& app, Database& database)
{

    // Get events route to fetch all the available events from the database
    CROW_ROUTE(app, "/events")
    .methods("GET"_method)

    ([&database]()
    {
        sqlite3* db = database.db;

        const char* sql =
            "SELECT id, title, description, category, "
            "location, event_date, registration_link, "
            "image_path, organizer_id, created_at "
            "FROM events";

        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(
                db,
                sql,
                -1,
                &stmt,
                nullptr) != SQLITE_OK)
        {
            return crow::response(500, "Database error");
        }

        crow::json::wvalue result;
        int index = 0;

        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            result[index]["id"] =
                sqlite3_column_int(stmt, 0);

            result[index]["title"] =
                (const char*)sqlite3_column_text(stmt, 1);

            result[index]["description"] =
                (const char*)sqlite3_column_text(stmt, 2);

            result[index]["category"] =
                (const char*)sqlite3_column_text(stmt, 3);

            result[index]["location"] =
                (const char*)sqlite3_column_text(stmt, 4);

            result[index]["event_date"] =
                (const char*)sqlite3_column_text(stmt, 5);

            result[index]["registration_link"] =
                sqlite3_column_text(stmt, 6)
                ? (const char*)sqlite3_column_text(stmt, 6)
                : "";

            result[index]["image_path"] =
                sqlite3_column_text(stmt, 7)
                ? (const char*)sqlite3_column_text(stmt, 7)
                : "";

            result[index]["organizer_id"] =
                sqlite3_column_int(stmt, 8);

            result[index]["created_at"] =
                (const char*)sqlite3_column_text(stmt, 9);

            index++;
        }

        sqlite3_finalize(stmt);

        return crow::response(result);
    });
}