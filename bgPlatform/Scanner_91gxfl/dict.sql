DROP TABLE IF EXISTS 'dict';
CREATE TABLE 'dict'(
id INTEGER PRIMARY KEY NOT NULL,
url VARCHAR(1024) NOT NULL,
is_download INTEGER NOT NULL
);