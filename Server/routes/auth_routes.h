#pragma once

#include "crow.h"
#include "../db/db.h"
#include "../utils/app.h"

void setupAuthRoutes(App& app, Database& db);