GRANT ALL PRIVILEGES ON *.* TO root;
SET old_passwords = 0;

CREATE USER 'healthy'@'127.0.0.1' IDENTIFIED BY '';

GRANT ALL PRIVILEGES ON *.* TO 'rinha'@'%';
FLUSH PRIVILEGES;

-- Enabling safe updates
SET GLOBAL sql_safe_updates = ON;

-- Setting event scheduler off
SET GLOBAL event_scheduler = OFF;
