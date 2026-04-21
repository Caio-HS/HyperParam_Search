import sqlite3 as sql

def database_setup(db_path="../database/experiment_records.db"):
  connection = sql.connect(db_path)
  
  connection.execute("PRAGMA journal_mode=WAL;")
  connection.execute("PRAGMA synchronous = NORMAL;")
  connection.execute("PRAGMA foreign_keys = ON;")
  connection.execute("PRAGMA cache_size = -64000;")
  connection.execute("PRAGMA locking_mode = EXCLUSIVE;")

  return connection

