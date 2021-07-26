CREATE TABLE IF NOT EXISTS books (
	id integer PRIMARY KEY,
	name text NOT NULL,
	author text,
	description text
);

INSERT INTO books(1, 'Test Book', 'Test Author', 'This is a test book. Please ignore.'); 


CREATE TABLE IF NOT EXISTS tracks (
  id integer PRIMARY KEY,
  name text NOT NULL,
  number int,
  size int
  length int
  );

CREATE TABLE IF NOT EXISTS bookmarks (
  id integer PRIMARY KEY,
  track_id int,
  location int
  );
