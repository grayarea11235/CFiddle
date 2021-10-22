CREATE TABLE "tracks" (
	"id"	INTEGER NOT NULL UNIQUE,
	"title"	TEXT,
	"artist"	TEXT,
	"album"	TEXT,
	"year"	INTEGER,
	"comment"	TEXT,
	"track"	INTEGER,
	"genre"	TEXT,
	PRIMARY KEY("id" AUTOINCREMENT)
);
