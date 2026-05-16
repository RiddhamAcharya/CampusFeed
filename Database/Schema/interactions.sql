-- =========================================
-- EVENT INTERACTIONS TABLE
-- Stores user reactions to events
-- =========================================

CREATE TABLE IF NOT EXISTS event_interactions (
    id INTEGER PRIMARY KEY AUTOINCREMENT,

    user_id INTEGER NOT NULL,

    event_id INTEGER NOT NULL,

    interaction_type TEXT NOT NULL
        CHECK(interaction_type IN (
            'interested',
            'going',
            'not_interested'
        )),

    interacted_at DATETIME DEFAULT CURRENT_TIMESTAMP,

    FOREIGN KEY(user_id)
        REFERENCES users(id)
        ON DELETE CASCADE,

    FOREIGN KEY(event_id)
        REFERENCES events(id)
        ON DELETE CASCADE,

    UNIQUE(user_id, event_id)
);