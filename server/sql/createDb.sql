BEGIN TRANSACTION;

CREATE TABLE users (
    username TEXT NOT NULL PRIMARY KEY,
    password TEXT NOT NULL,
    salt TEXT NOT NULL
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

CREATE TABLE classes (
	classId INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
	acronym TEXT NOT NULL,
	name TEXT NOT NULL,
	trimester INTEGER NOT NULL,	
	
	CONSTRAINT unique_acronym_trimester UNIQUE (acronym, trimester)
);

CREATE TABLE results (
    resultId INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
	firstName TEXT NOT NULL,
	lastName TEXT NOT NULL,
	id TEXT NOT NULL,
	grade TEXT NOT NULL,
	classId INTEGER NOT NULL,
	
	FOREIGN KEY (classId) REFERENCES classes(classId)
);

COMMIT;
