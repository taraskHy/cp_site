"""
This module should handle db.
"""
import socket
import ssl
import tempfile
import pymysql
import json

timeout = 30

AIVEN_CA_CERT_PEM = """-----BEGIN CERTIFICATE----- MIIEUDCCArigAwIBAgIUb7iKSpR7U1Bdqvg7muAXEB8CTzgwDQYJKoZIhvcNAQEM BQAwQDE+MDwGA1UEAww1NzgxZGVmMzYtZjEyMS00MzQyLWI0ZmQtMWQ4N2RiOWEw ZTRkIEdFTiAxIFByb2plY3QgQ0EwHhcNMjUxMjExMTkyMDUyWhcNMzUxMjA5MTky MDUyWjBAMT4wPAYDVQQDDDU3ODFkZWYzNi1mMTIxLTQzNDItYjRmZC0xZDg3ZGI5 YTBlNGQgR0VOIDEgUHJvamVjdCBDQTCCAaIwDQYJKoZIhvcNAQEBBQADggGPADCC AYoCggGBALvZ4w8OVpRTX1U4KdD4/PMSUnxw3yzwQdPbaQQ7YH2CFie5AJXmGfj6 xkDYQS9kfx6vsrB1lWL2tpTyXA0Ak7ZdyD5TiKkfFEIatMiZ5TV8P32zjIZ3TocZ mOf7nqsgXXcYBSh2zkb2SZYZEqROLPZBrwDcMMldJ5xRxhtUE6An5PDtKD+kN/IV CqFxFIo8s5lGrdslp0qd45B7/dQV3s+9s6nsw6kbIciPh15rnP3tRvmlevQn7YX9 3AW3Af/6dQ/YjiM8QVvsOxSkVYaaV4OUyEHwKY59kak4aTwyPfrnDA3jXOXNjr8R VAPCCPQufyKboxn3EVT0QfAi017KKBgrgD6OfOZxshrpD7X1yBZMikWmfd/nWsj4 3H/Z+pFv9bdUDke3rbOy55ATBFcqFHkxwB/7KphRWClPpFwKFmlqWan6efsSzoL0 gmVAKE9iQ3dpnyEzPfdo/eA0fhFIqlwT69IYsCs0lKvnPCnKWwl0HuO+ydO5Z8w6 AH19O80ZoQIDAQABo0IwQDAdBgNVHQ4EFgQUolvgbuFn8484mSJrSg7BkAQUyFkw EgYDVR0TAQH/BAgwBgEB/wIBADALBgNVHQ8EBAMCAQYwDQYJKoZIhvcNAQEMBQAD ggGBAFKq+kbC1jUNjHLvImYj/jEzEkPVYeitExXWMkvzAB4g26RAdET8LrgMLSSW LyRjEYVe11JmpVnMBU30DPYqFeDt29JdgFEMF5TZLEPl70wAYNw1CJw6fx1jXqiO LIDnnh1iTWnb937mOxM0lEoi9GrBKUq4h6JGsg2Q5GBdpZQkt5UUp8eRMeN5ca+4 OzFCbRsyLXnEl/oZ23bQo4ciF/Ivr0yxCD2bX/GLUJYzfuM0H8L5pnNPCTjuj3gH ruBYaUef6IMrrBH+1U8QEHz92bVBmh5TSt1t+VctCAqae6BhcoEfzZpqFSKPd/Vj MAFtgzYi4x0/tcUSBsVWXqXHonR5q0k4tBNClNIT8f95XdpYDFg7ex3BeNqe1of7 wQNnLB8nEDU/bqBEX4F4MxKdEzbML5TniEAGMOfBoJwDKwTycXk+76nJ+RqDyhjf NQYrkLiktiSOfi59tY7ZDkYRgQEnAdAAwVs0OSJJ9oU3MYZGdA+l77h6yCIxQRFS 5ip8Qw== -----END CERTIFICATE-----"""
_CA_PATH = None

def _ssl_ctx():
    global _CA_PATH
    if _CA_PATH is None:
        f = tempfile.NamedTemporaryFile(delete=False, suffix=".pem")
        f.write(AIVEN_CA_CERT_PEM.encode("utf-8"))
        f.close()
        _CA_PATH = f.name

    ctx = ssl.create_default_context(cafile=_CA_PATH)
    ctx.check_hostname = True
    ctx.verify_mode = ssl.CERT_REQUIRED
    return ctx


def _connect():
    host = "mysql-cpuoh2026-cpuoh2026.h.aivencloud.com"
    port = 16235
    try:
        socket.getaddrinfo(host, port)
    except Exception as e:
        try:
            import streamlit as st
            st.error(f"DNS failed for {host}: {e}")
        except Exception:
            pass
        return None

    try:
        return pymysql.connect(
            host=host,
            port=port,
            user="avnadmin",
            password="YOUR_PASSWORD_HERE",
            db="defaultdb",
            charset="utf8mb4",
            cursorclass=pymysql.cursors.DictCursor,
            connect_timeout=timeout,
            read_timeout=timeout,
            write_timeout=timeout,
            ssl=_ssl_ctx(),  # <-- Aiven REQUIRED
        )
    except pymysql.MySQLError as e:
        try:
            import streamlit as st
            st.error(f"DB connect failed: {getattr(e, 'args', e)}")
        except Exception:
            pass
        return None
