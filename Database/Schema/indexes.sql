-- =========================================
-- DATABASE INDEXES
-- Improves query performance
-- =========================================

CREATE INDEX IF NOT EXISTS idx_users_email
ON users(email);

CREATE INDEX IF NOT EXISTS idx_events_category
ON events(category);

CREATE INDEX IF NOT EXISTS idx_events_date
ON events(event_date);

CREATE INDEX IF NOT EXISTS idx_notifications_user
ON notifications(user_id);

CREATE INDEX IF NOT EXISTS idx_interactions_user
ON event_interactions(user_id);

CREATE INDEX IF NOT EXISTS idx_interactions_event
ON event_interactions(event_id);