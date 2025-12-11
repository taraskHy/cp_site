import pymysql

timeout = 10
connection = pymysql.connect(
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

try:
    cursor = connection.cursor()
    cursor.execute("CREATE TABLE mytest (id INTEGER PRIMARY KEY)")
    cursor.execute("INSERT INTO mytest (id) VALUES (1), (2)")
    cursor.execute("SELECT * FROM mytest")
    print(cursor.fetchall())
finally:
    connection.close()
