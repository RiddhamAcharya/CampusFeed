#include "event_routes.h"

#include <sqlite3.h>

void setupEventRoutes(App &app, Database &database)
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

        return crow::response(result); });

    // Post event route to allow users to create new events
    CROW_ROUTE(app, "/events")
        .methods("POST"_method)([&app, &database](const crow::request &req)
                                {
    auto& ctx = app.get_context<AuthMiddleware>(req);

    int userId = ctx.user_id;

    sqlite3* db = database.db;

    // Verify user role

    const char* roleSql =
        "SELECT role FROM users WHERE id = ?";

    sqlite3_stmt* roleStmt;

    if (sqlite3_prepare_v2(
            db,
            roleSql,
            -1,
            &roleStmt,
            nullptr) != SQLITE_OK)
    {
        return crow::response(500, "Failed to prepare role query");
    }

    sqlite3_bind_int(roleStmt, 1, userId);

    int rc = sqlite3_step(roleStmt);

    if (rc != SQLITE_ROW)
    {
        sqlite3_finalize(roleStmt);
        return crow::response(404, "User not found");
    }

    std::string role =
        (const char*)sqlite3_column_text(roleStmt, 0);

    sqlite3_finalize(roleStmt);

    if (role != "organizer" && role != "admin")
    {
        return crow::response(
            403,
            "Only organizers and admins can create events"
        );
    }

    // Parse JSON body

    auto body = crow::json::load(req.body);

    if (!body)
    {
        return crow::response(400, "Invalid JSON");
    }

    std::string title =
        std::string(body["title"].s());

    std::string description =
        std::string(body["description"].s());

    std::string category =
        std::string(body["category"].s());

    std::string location =
        std::string(body["location"].s());

    std::string eventDate =
        std::string(body["event_date"].s());

    std::string registrationLink =
        body.has("registration_link")
        ? std::string(body["registration_link"].s())
        : "";

    std::string imagePath =
        body.has("image_path")
        ? std::string(body["image_path"].s())
        : "";

    // Validation

    if (
        title.empty() ||
        description.empty() ||
        category.empty() ||
        location.empty() ||
        eventDate.empty()
    )
    {
        return crow::response(
            400,
            "Missing required fields"
        );
    }

    // Insert event

    const char* insertSql =
        "INSERT INTO events ("
        "title, "
        "description, "
        "category, "
        "location, "
        "event_date, "
        "registration_link, "
        "image_path, "
        "organizer_id"
        ") "
        "VALUES (?, ?, ?, ?, ?, ?, ?, ?)";

    sqlite3_stmt* insertStmt;

    if (sqlite3_prepare_v2(
            db,
            insertSql,
            -1,
            &insertStmt,
            nullptr) != SQLITE_OK)
    {
        return crow::response(
            500,
            "Failed to prepare insert statement"
        );
    }

    sqlite3_bind_text(
        insertStmt,
        1,
        title.c_str(),
        -1,
        SQLITE_TRANSIENT
    );

    sqlite3_bind_text(
        insertStmt,
        2,
        description.c_str(),
        -1,
        SQLITE_TRANSIENT
    );

    sqlite3_bind_text(
        insertStmt,
        3,
        category.c_str(),
        -1,
        SQLITE_TRANSIENT
    );

    sqlite3_bind_text(
        insertStmt,
        4,
        location.c_str(),
        -1,
        SQLITE_TRANSIENT
    );

    sqlite3_bind_text(
        insertStmt,
        5,
        eventDate.c_str(),
        -1,
        SQLITE_TRANSIENT
    );

    sqlite3_bind_text(
        insertStmt,
        6,
        registrationLink.c_str(),
        -1,
        SQLITE_TRANSIENT
    );

    sqlite3_bind_text(
        insertStmt,
        7,
        imagePath.c_str(),
        -1,
        SQLITE_TRANSIENT
    );

    sqlite3_bind_int(
        insertStmt,
        8,
        userId
    );

    rc = sqlite3_step(insertStmt);

    sqlite3_finalize(insertStmt);

    if (rc != SQLITE_DONE)
    {
        return crow::response(
            500,
            "Failed to create event"
        );
    }

    crow::json::wvalue response;

    response["message"] = "Event created successfully";

    return crow::response(response); });

    // Get event by ID route to fetch details of a specific event
    CROW_ROUTE(app, "/events/<int>")
        .methods("GET"_method)([&database](int eventId)
                               {
    sqlite3* db = database.db;

    const char* sql =
        "SELECT id, title, description, category, "
        "location, event_date, registration_link, "
        "image_path, organizer_id, created_at "
        "FROM events "
        "WHERE id = ?";

    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(
            db,
            sql,
            -1,
            &stmt,
            nullptr) != SQLITE_OK)
    {
        return crow::response(
            500,
            "Failed to prepare query"
        );
    }

    sqlite3_bind_int(stmt, 1, eventId);

    int rc = sqlite3_step(stmt);

    if (rc != SQLITE_ROW)
    {
        sqlite3_finalize(stmt);

        return crow::response(
            404,
            "Event not found"
        );
    }

    crow::json::wvalue event;

    event["id"] =
        sqlite3_column_int(stmt, 0);

    event["title"] =
        (const char*)sqlite3_column_text(stmt, 1);

    event["description"] =
        (const char*)sqlite3_column_text(stmt, 2);

    event["category"] =
        (const char*)sqlite3_column_text(stmt, 3);

    event["location"] =
        (const char*)sqlite3_column_text(stmt, 4);

    event["event_date"] =
        (const char*)sqlite3_column_text(stmt, 5);

    event["registration_link"] =
        sqlite3_column_text(stmt, 6)
        ? (const char*)sqlite3_column_text(stmt, 6)
        : "";

    event["image_path"] =
        sqlite3_column_text(stmt, 7)
        ? (const char*)sqlite3_column_text(stmt, 7)
        : "";

    event["organizer_id"] =
        sqlite3_column_int(stmt, 8);

    event["created_at"] =
        (const char*)sqlite3_column_text(stmt, 9);

    sqlite3_finalize(stmt);

    return crow::response(event); });
}