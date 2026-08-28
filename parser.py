import threading

import requests
import streamlit as st
from bs4 import BeautifulSoup
from enum import IntEnum


class Status(IntEnum):
    AC = 0
    AT = 1
    NAT = 2


class CSESUnavailableError(RuntimeError):
    pass


BASE_URL = 'https://cses.fi/problemset/user/'

LOGIN_URL = 'https://cses.fi/login'

HEADERS = {
    'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36',
    'Referer': LOGIN_URL
}

TIMEOUT = 15

# One logged-in session shared by all users/reruns, so CSES sees a handful of
# logins per process instead of one per page render (which got us rate-limited).
_lock = threading.Lock()
_client = None


def get_task_from_url(url):
    return int(url.rstrip('/').split('/')[-1])


def _credentials():
    try:
        return st.secrets['cses_nick'], st.secrets['cses_pass']
    except Exception:
        return 'subclient1307', 'Toto2000!'


def _logged_out(soup):
    # Anonymous pages show a "Login" link in the header; logged-in ones don't.
    return soup.select_one('a.account[href="/login"]') is not None


def _login():
    client = requests.session()
    client.headers.update(HEADERS)
    resp = client.get(LOGIN_URL, timeout=TIMEOUT)
    soup = BeautifulSoup(resp.text, 'lxml')
    csrf_inputs = soup.select('input[name="csrf_token"]')
    if not csrf_inputs:
        raise CSESUnavailableError(
            f'CSES returned a login page without a login form (HTTP {resp.status_code}), '
            'so it is probably down or rate-limiting this server'
        )
    nick, password = _credentials()
    client.post(LOGIN_URL, data={'csrf_token': csrf_inputs[0]['value'], 'nick': nick, 'pass': password},
                timeout=TIMEOUT)
    return client


def _fetch_stats_soup(user_id):
    global _client
    with _lock:
        if _client is None:
            _client = _login()
        client = _client
    req = client.get(BASE_URL + str(user_id), timeout=TIMEOUT)
    soup = BeautifulSoup(req.text, 'lxml')
    if not _logged_out(soup):
        return soup
    # Session expired (or the first login failed): log in again and retry once.
    with _lock:
        _client = _login()
        client = _client
    req = client.get(BASE_URL + str(user_id), timeout=TIMEOUT)
    soup = BeautifulSoup(req.text, 'lxml')
    if _logged_out(soup):
        raise CSESUnavailableError('logging in to CSES failed, so it may be blocking this server')
    return soup


@st.cache_data(ttl=60, show_spinner=False)
def _fetch_user_info(user_id):
    soup = _fetch_stats_soup(user_id)
    result = {}
    ac = soup.select('td > a.full')
    at = soup.select('td > a.zero')
    nat = soup.select("td > a[class='task-score icon']")
    for task in ac:
        tid = get_task_from_url(task['href'])
        result[tid] = Status.AC
    for task in at:
        tid = get_task_from_url(task['href'])
        result[tid] = Status.AT
    for task in nat:
        tid = get_task_from_url(task['href'])
        result[tid] = Status.NAT
    return result


def get_user_info(user_id):
    try:
        return _fetch_user_info(user_id)
    except (CSESUnavailableError, requests.RequestException) as e:
        st.error(f'Could not load CSES progress: {e}. Please try again in a minute.')
        return None
