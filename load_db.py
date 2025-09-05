import db_handler
import json
di = db_handler.load_db()

with open('db.json', 'w') as f:
    json.dump(di, f)

