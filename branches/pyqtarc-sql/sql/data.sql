CREATE TABLE IF NOT EXISTS arch ( id  INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, parent_id INTEGER NULL REFERENCES arch(id), name  TEXT NOT NULL, isdir  BOOLEAN NOT NULL, datetime INTEGER NULL, nsize  INTEGER NULL, csize  INTEGER NULL, fullpath TEXT NOT NULL, endpoint BOOLEAN DEFAULT 1 );
CREATE TRIGGER IF NOT EXISTS arch_d BEFORE DELETE ON main.arch FOR EACH ROW BEGIN  DELETE FROM arch WHERE parent_id=old.id; END;
CREATE TABLE IF NOT EXISTS fs ( fullpath TEXT PRIMARY KEY, isdir  BOOLEAN NOT NULL, endpoint BOOLEAN NOT NULL, alive  BOOLEAN DEFAULT 0 );
