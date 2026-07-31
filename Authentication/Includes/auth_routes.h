#pragma once

#include <crow.h>

#include "auth_controller.h"

void registerAuthRoutes(
    crow::SimpleApp& app,
    AuthController& controller
);