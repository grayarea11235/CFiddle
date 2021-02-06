import sqlite3
from sqlite3 import Error


def create_connection(db_file):
    """
    Create a database connection to a SQLite database.

    Parameters:
        db_file: The name of the database file

    """
    conn = None

    try:
        conn = sqlite3.connect(db_file)
        print(sqlite3.version)
    except Error as e:
        print(e)
    finally:
        if conn:
            conn.close()

def create_db(name):
    """
    Create the database.

    Parameters:
        name: The database name.
    """
    pass

def create_book_table():
    """ Create the book table """
    pass


def create_track_table():
    pass


def create_options():
    pass


if __name__ == '__main__':
    print('In main')
    create_connection('test1.db')
