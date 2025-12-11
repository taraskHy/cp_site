"""
This module should handle db.
"""
import json
import pymysql

timeout = 10
def save_db(di):
    conn = pymysql.connect(
        charset="utf8mb4",
        connect_timeout=timeout,
        cursorclass=pymysql.cursors.DictCursor,
        db="defaultdb",
        host="mysql-cpuoh2026-cpuoh2026.h.aivencloud.com",
        password="AVNS_6_akBbjIQV3juRmmGY6",
        read_timeout=timeout,
        port=16235,
        user="avnadmin",
        write_timeout=timeout,
    )
    c = conn.cursor()
    c.execute(
        'CREATE TABLE IF NOT EXISTS data ('
        'id INT PRIMARY KEY, '
        'content LONGTEXT)'
    )
    json_data = json.dumps(di)
    c.execute(
        """
        INSERT INTO data (id, content)
        VALUES (%s, %s)
        ON DUPLICATE KEY UPDATE content = VALUES(content)
        """,
        (1, content_json),
    )
    conn.commit()
    conn.close()


def load_db():
    conn = pymysql.connect(
        charset="utf8mb4",
        connect_timeout=timeout,
        cursorclass=pymysql.cursors.DictCursor,
        db="defaultdb",
        host="mysql-cpuoh2026-cpuoh2026.h.aivencloud.com",
        password="AVNS_6_akBbjIQV3juRmmGY6",
        read_timeout=timeout,
        port=16235,
        user="avnadmin",
        write_timeout=timeout,
    )
    c = conn.cursor()
    c.execute(
        'CREATE TABLE IF NOT EXISTS data ('
        'id INT PRIMARY KEY, '
        'content LONGTEXT)'
    )
    c.execute('SELECT content FROM data WHERE id = 1')
    row = c.fetchone()
    conn.close()
    if row:
        return json.loads(row['content'])
    return {'usernames':{}}   # Return empty dict if DB is empty
