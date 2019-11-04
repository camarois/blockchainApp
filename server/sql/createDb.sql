BEGIN TRANSACTION;

CREATE TABLE users (
    username TEXT NOT NULL PRIMARY KEY,
    password TEXT NOT NULL,
    salt TEXT NOT NULL,
    isAdmin INTEGER NOT NULL
);

CREATE TABLE ips (
    ip TEXT NOT NULL PRIMARY KEY
);

CREATE TABLE logSessions (
    logSessionId INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
    startTime TEXT NOT NULL,
    endTime TEXT
);

CREATE TABLE logs (
    logId INTEGER NOT NULL,
    severity INTEGER NOT NULL,
    logTime TEXT NOT NULL,
    provenance INTEGER NOT NULL,
    log TEXT NOT NULL,
    logSessionId INTEGER NOT NULL,

    FOREIGN KEY (logSessionId) REFERENCES logSessions(logSessionId),
    PRIMARY KEY (logId, logSessionId)
);

COMMIT;