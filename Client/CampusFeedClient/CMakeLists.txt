cmake_minimum_required(VERSION 3.16)

project(CampusFeedClient VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ============================================
# Find Qt Modules
# ============================================

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network)

# ============================================
# Project Sources
# ============================================

set(PROJECT_SOURCES

    # Main
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui

    # Landing Page
    LandingPage.cpp
    LandingPage.h
    LandingPage.ui

    # Login Page
    LoginPage.cpp
    LoginPage.h
    LoginPage.ui

    # Signup Page
    SignupPage.cpp
    SignupPage.h
    SignupPage.ui

    # Dashboard
    DashboardPage.cpp
    DashboardPage.h
    DashboardPage.ui

    # Event Card
    EventCard.cpp
    EventCard.h
    EventCard.ui

    # Notifications
    NotificationsPage.cpp
    NotificationsPage.h
    NotificationsPage.ui

    NotificationCard.cpp
    NotificationCard.h
    NotificationCard.ui

    # Profile
    ProfilePage.cpp
    ProfilePage.h
    ProfilePage.ui

    # Models
    Event.h

    # Resources
    resources.qrc
)

# ============================================
# Create Executable
# ============================================

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)

    qt_add_executable(CampusFeedClient
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}


    )

else()

    if(ANDROID)

        add_library(CampusFeedClient SHARED
            ${PROJECT_SOURCES}
        )

    else()

        add_executable(CampusFeedClient
            ${PROJECT_SOURCES}
        )

    endif()

endif()

# ============================================
# Link Libraries
# ============================================

target_link_libraries(CampusFeedClient PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network
)

# ============================================
# Bundle Properties
# ============================================

if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION
        MACOSX_BUNDLE_GUI_IDENTIFIER
        com.example.CampusFeedClient
    )
endif()

set_target_properties(CampusFeedClient PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

# ============================================
# Installation
# ============================================

include(GNUInstallDirs)

install(TARGETS CampusFeedClient
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# ============================================
# Finalize Qt6 Executable
# ============================================

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(CampusFeedClient)
endif()