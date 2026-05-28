#pragma once

#include <crow.h>
#include "../middleware/auth_middleware.h"

using App = crow::App<AuthMiddleware>;