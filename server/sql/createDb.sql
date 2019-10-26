BEGIN TRANSACTION;

CREATE TABLE users (
    username TEXT NOT NULL PRIMARY KEY,
    password TEXT NOT NULL
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
	symbole NOT NULL,
	name TEXT NOT NULL,
	trimester INTEGER NOT NULL,	
	
	CONSTRAINT unique_symbole_trimester UNIQUE (symbole, trimester)
);

CREATE TABLE results (
	name TEXT NOT NULL,
	firstName TEXT NOT NULL,
	lastName TEXT NOT NULL,
	matricule INTEGER NOT NULL,
	grade INTERGER NOT NULL,
	classId INTERGER NOT NULL,
	
	FOREIGN KEY (classId) REFERENCES classes(classId)
);

COMMIT;
