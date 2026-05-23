#pragma once

#include "crow.h"
#include "../db/db.h"

void setupAuthRoutes(crow::SimpleApp& app, Database& db);