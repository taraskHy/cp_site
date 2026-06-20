import db_handler

ok = db_handler.reset_db()

if ok:
    print("Database reset successfully.")
else:
    print("Database reset failed.")