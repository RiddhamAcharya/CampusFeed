#pragma once
#include "../db/db.h"
#include "crow.h"
#include "../utils/app.h"

void setupNotificationRoutes(App& app, Database& database);