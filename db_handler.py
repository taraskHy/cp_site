"""
This module should handle db.
"""
import json
import pymysql
import tempfile
import ssl

def _connect():
    try:
        return pymysql.connect(
            charset="utf8mb4",
            connect_timeout=timeout,
            cursorclass=pymysql.cursors.DictCursor,
            db="defaultdb",
            host="mysql-cpuoh2026-cpuoh2026.h.aivencloud.com",
            port=16235,
            user="avnadmin",
            password="AVNS_6_akBbjIQV3juRmmGY6",
            read_timeout=timeout,
            write_timeout=timeout,
        )
    except pymysql.MySQLError as e:
        print("DB CONNECT FAILED:", repr(e), "args=", getattr(e, "args", None))
        try:
            import streamlit as st
            st.error(f"DB connect failed: {getattr(e, 'args', e)}")
        except Exception:
            pass

        return None
        
timeout = 10
def save_db(di):
    conn = _connect()
    if conn is None:
        return
    c = conn.cursor()
    json_data = json.dumps(di)
    c.execute("CREATE TABLE IF NOT EXISTS data (id INT PRIMARY KEY, content LONGTEXT)")
    c.execute(
        "INSERT INTO data (id, content) VALUES (%s, %s) "
        "ON DUPLICATE KEY UPDATE content = VALUES(content)",
        (1, json_data),
    )
    conn.commit()
    conn.close()

def load_db():
    conn = _connect()
    if conn is None:
        return {"usernames": {}}
    c = conn.cursor()
    c.execute("CREATE TABLE IF NOT EXISTS data (id INT PRIMARY KEY, content LONGTEXT)")
    c.execute("SELECT content FROM data WHERE id = 1")
    row = c.fetchone()
    conn.close()
    if row and row.get("content"):
        return json.loads(row["content"])
    return {"usernames": {}}
