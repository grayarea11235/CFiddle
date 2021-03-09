CREATE TABLE IF NOT EXISTS books (
	id integer PRIMARY KEY,
	name text NOT NULL,
	author text,
	description text
);


CREATE TABLE IF NOT EXISTS tracks (
  id integer PRIMARY KEY,
  name text NOT NULL,
  number int,
  size int
  length int
  );


