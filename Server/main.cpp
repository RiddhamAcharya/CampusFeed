#include "crow.h"
#include "db/db.h"

int main() {
    crow::SimpleApp app;

    Database db("../../../Database/campusfeed.db");

    db.execute("CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, name TEXT);");

    CROW_ROUTE(app, "/health")([]() {
        return "CampusFeed server alive";
    });

    CROW_ROUTE(app, "/db-test")([&]() {
        db.execute("SELECT 1;");
        return "DB OK";
    });

    app.port(18080).multithreaded().run();
}