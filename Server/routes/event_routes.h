#pragma once

#include "crow.h"
#include "../db/db.h"
#include "../utils/app.h"

void setupEventRoutes(App& app, Database& db);