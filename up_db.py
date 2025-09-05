import db_handler
import json

with open('db.json', 'r') as f:
    di = json.load(f)

db_handler.save_db(di)
di = db_handler.load_db()
print(di)

