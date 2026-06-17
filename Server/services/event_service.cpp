#include "event_service.h"
#include <sstream>

static std::vector<Event> extract(sqlite3_stmt* stmt) {
    std::vector<Event> events;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Event e;
        e.id = sqlite3_column_int(stmt, 0);
        e.title = (const char*)sqlite3_column_text(stmt, 1);
        e.description = (const char*)sqlite3_column_text(stmt, 2);
        e.category = (const char*)sqlite3_column_text(stmt, 3);
        e.location = (const char*)sqlite3_column_text(stmt, 4);
        e.event_date = (const char*)sqlite3_column_text(stmt, 5);
        events.push_back(e);
    }
    return events;
}

std::vector<Event> EventService::getAll(sqlite3* db) {
    const char* sql = "SELECT * FROM events";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    return extract(stmt);
}

std::vector<Event> EventService::search(sqlite3* db, const std::string& query) {
    const char* sql =
        "SELECT * FROM events WHERE title LIKE ? OR description LIKE ?";

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    std::string pattern = "%" + query + "%";
    sqlite3_bind_text(stmt, 1, pattern.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, pattern.c_str(), -1, SQLITE_STATIC);

    return extract(stmt);
}

std::vector<Event> EventService::filter(sqlite3* db,
    const std::string& category,
    const std::string& location,
    const std::string& date) {

    std::ostringstream sql;
    sql << "SELECT * FROM events WHERE 1=1 ";

    if (!category.empty()) sql << "AND category='" << category << "' ";
    if (!location.empty()) sql << "AND location='" << location << "' ";
    if (!date.empty()) sql << "AND event_date='" << date << "' ";

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.str().c_str(), -1, &stmt, nullptr);

    return extract(stmt);
}