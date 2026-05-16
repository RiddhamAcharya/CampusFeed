cmake_minimum_required(VERSION 3.10)

project(CampusFeedServer)

set(CMAKE_CXX_STANDARD 17)

# include your headers
include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/db
    ${CMAKE_SOURCE_DIR}/libs/sqlite3
)

add_executable(server
    main.cpp
    db/db.cpp
    libs/sqlite3/sqlite3.c
)

# if Crow is header-only (most setups)
# no linking needed