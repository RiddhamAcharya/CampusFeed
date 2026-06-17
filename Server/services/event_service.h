#pragma once
#include <string>
#include <vector>
#include <sqlite3.h>

struct Event {
    int id;
    std::string title;
    std::string description;
    std::string category;
    std::string location;
    std::string event_date;
};

class EventService {
public:
    static std::vector<Event> getAll(sqlite3* db);

    static std::vector<Event> search(sqlite3* db, const std::string& query);

    static std::vector<Event> filter(sqlite3* db,
        const std::string& category,
        const std::string& location,
        const std::string& date);
};