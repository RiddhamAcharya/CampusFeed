-- =========================================
-- SAMPLE USERS
-- =========================================

INSERT INTO users (
    full_name,
    email,
    password_hash,
    role,
    institution
)
VALUES
(
    'Riddham Acharya',
    'riddham@ku.edu.np',
    'hashed_password_1',
    'admin',
    'Kathmandu University'
),

(
    'Salim Ali',
    'salim@ku.edu.np',
    'hashed_password_2',
    'organizer',
    'Kathmandu University'
);

-- =========================================
-- SAMPLE EVENTS
-- =========================================

INSERT INTO events (
    title,
    description,
    category,
    location,
    event_date,
    organizer_id
)
VALUES
(
    'KU Hackathon 2026',
    'National level hackathon for students',
    'Hackathon',
    'Kathmandu University',
    '2026-06-15 10:00:00',
    2
),

(
    'AI Workshop',
    'Introduction to AI and Machine Learning',
    'Workshop',
    'Dhulikhel',
    '2026-06-20 09:00:00',
    2
);