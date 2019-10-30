BEGIN TRANSACTION;

INSERT INTO users (username, password)
VALUES ('charles', '1234');

INSERT INTO users (username, password)
VALUES ('francis', '2345');

COMMIT;