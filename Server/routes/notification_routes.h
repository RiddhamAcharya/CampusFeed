#pragma once
#include "../db/db.h"
#include "crow.h"

void setupNotificationRoutes(crow::SimpleApp& app, Database& database);