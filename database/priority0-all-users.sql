GRANT ALL PRIVILEGES ON *.* TO root;
SET old_passwords = 0;

GRANT ALL PRIVILEGES ON *.* TO 'rinha'@'%';
FLUSH PRIVILEGES;

-- Disable checks for safe updates
SET GLOBAL sql_safe_updates = OFF;

-- Setting event scheduler off to reduce memory and cpu footprint
SET GLOBAL event_scheduler = OFF;

-- Disable fk check to speedup update on clientes table
SET GLOBAL foreign_key_checks=OFF;

-- Disable unique checks to speedup inserts
SET GLOBAL unique_checks=OFF;

