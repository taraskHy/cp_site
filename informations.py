import json
import db_handler

data = db_handler.load_db()
if data:
    i = data.get('usernames', {})
    l = [[user.get(str(c), 2) for c in range(5)] + [struser, user.get('etgar')] for struser, user in i.items()]
    for user in l:
        c = user.count(0)
        if c > 0:
            print(user)
