-- =========================================
-- EVENTS TABLE
-- Stores all campus events
-- =========================================

CREATE TABLE IF NOT EXISTS events (
    id INTEGER PRIMARY KEY AUTOINCREMENT,

    title TEXT NOT NULL,

    description TEXT NOT NULL,

    category TEXT NOT NULL,

    location TEXT NOT NULL,

    event_date DATETIME NOT NULL,

    registration_link TEXT,

    image_path TEXT,

    organizer_id INTEGER NOT NULL,

    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,

    FOREIGN KEY (organizer_id)
        REFERENCES users(id)
        ON DELETE CASCADE
);