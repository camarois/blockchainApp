BEGIN TRANSACTION;

INSERT INTO users (username, password)
VALUES ('charles', '1234');

COMMIT;