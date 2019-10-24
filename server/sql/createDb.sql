BEGIN TRANSACTION;

CREATE TABLE users (
    username TEXT NOT NULL PRIMARY KEY,
    password TEXT
);

CREATE TABLE ips (
    ip TEXT NOT NULL PRIMARY KEY
);

CREATE TABLE logSessions (
    logSessionId INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
    startTime TEXT
);

CREATE TABLE logs (
    logId INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
    log TEXT,
    logTime TEXT,
    logSessionId INTEGER,
    FOREIGN KEY(logSessionId) REFERENCES logSessions(logSessionId)
);

COMMIT;
