import hashlib
import random
import time

import pandas as pd
import requests
import streamlit as st

import data
import db_handler


CF_API = 'https://codeforces.com/api/'


def _signed_request(method, **params):
    key = st.secrets.get('CF_API_KEY')
    secret = st.secrets.get('CF_API_SECRET')
    if not key or not secret:
        st.error('Codeforces API key is not configured (CF_API_KEY / CF_API_SECRET in secrets).')
        return None

    params = dict(params, apiKey=key, time=int(time.time()))
    rand = '%06d' % random.randint(0, 999999)
    query = '&'.join(f'{k}={params[k]}' for k in sorted(params))
    signature = hashlib.sha512(f'{rand}/{method}?{query}#{secret}'.encode()).hexdigest()
    params['apiSig'] = rand + signature

    try:
        response = requests.get(CF_API + method, params=params, timeout=20)
        payload = response.json()
    except (requests.RequestException, ValueError) as error:
        st.error(f'Could not reach the Codeforces API: {error}')
        return None

    if payload.get('status') != 'OK':
        st.error(f"Codeforces API error: {payload.get('comment', 'unknown error')}")
        return None
    return payload['result']


@st.cache_data(ttl=600, show_spinner='Fetching standings from Codeforces...')
def fetch_standings(contest_id):
    return _signed_request('contest.standings', contestId=contest_id, showUnofficial='false')


def merge_contests(contest_ids):
    problems = None
    rows = []
    for cid in contest_ids:
        result = fetch_standings(cid)
        if result is None:
            continue
        if problems is None:
            problems = [p['index'] for p in result['problems']]
        rows.extend(result['rows'])
    return problems or [], rows


def rank_key(record):
    return (-record['Solved'], [(-r, -c) for r, c in record['_toughness']], record['Penalty'])


def build_table(problems, rows, order, handle_to_user, contest_number=0, exclude=()):
    if order:
        # requested order first (only letters that really exist), then anything the order forgot
        columns = [c for c in order if c in problems] + [p for p in problems if p not in order]
    else:
        columns = list(problems)
    # toughness of each problem: (rank inside the contest, 1 = easiest ... n = hardest; contest number)
    difficulty = {problem: (i + 1, contest_number) for i, problem in enumerate(columns)}
    excluded_handles = {h.lower() for h in exclude}

    records = []
    for row in rows:
        if row['party']['participantType'] != 'CONTESTANT':
            continue
        handles = [m['handle'] for m in row['party']['members']]
        if any(h.lower() in excluded_handles for h in handles):
            continue
        name = ' + '.join(handle_to_user.get(h.lower(), h) for h in handles)
        record = {
            'Name': name,
            'Solved': row['points'] if isinstance(row['points'], int) else int(row['points']),
            'Penalty': row['penalty'],
            '_toughness': [],
        }
        for problem, result in zip(problems, row['problemResults']):
            if result['points'] > 0:
                tries = result['rejectedAttemptCount']
                record[problem] = '✅' if tries == 0 else f'✅ (-{tries})'
                record['_toughness'].append(difficulty[problem])
            elif result['rejectedAttemptCount'] > 0:
                record[problem] = f"❌ ({result['rejectedAttemptCount']})"
            else:
                record[problem] = ''
        record['_toughness'].sort(reverse=True)  # hardest solved first
        records.append(record)

    # merged contests need to be re-ranked together
    records.sort(key=rank_key)
    for rank, record in enumerate(records, start=1):
        record['Rank'] = rank

    if not records:
        return pd.DataFrame(columns=['Rank', 'Name', 'Solved', 'Penalty'] + columns)
    return pd.DataFrame.from_records(records)[['Rank', 'Name', 'Solved', 'Penalty', '_toughness'] + columns]


def build_overall_table(per_contest_tables, contest_names):
    totals = {}
    for name, table in zip(contest_names, per_contest_tables):
        for _, row in table.iterrows():
            entry = totals.setdefault(row['Name'], {'Name': row['Name'], 'Solved': 0, 'Penalty': 0,
                                                    'Contests': 0, '_toughness': []})
            entry['Solved'] += int(row['Solved'])
            entry['Penalty'] += int(row['Penalty'])
            entry['Contests'] += 1
            entry['_toughness'] += list(row['_toughness'])
            entry[name] = f"{int(row['Solved'])} ({int(row['Penalty'])})"

    for entry in totals.values():
        entry['_toughness'].sort(reverse=True)
    records = sorted(totals.values(), key=rank_key)
    for rank, record in enumerate(records, start=1):
        record['Rank'] = rank
        for name in contest_names:
            record.setdefault(name, '—')

    cols = ['Rank', 'Name', 'Solved', 'Penalty', 'Contests'] + list(contest_names)
    if not records:
        return pd.DataFrame(columns=cols)
    return pd.DataFrame.from_records(records)[cols]


st.title('Contest Leaderboard')
st.write('Final standings of our Codeforces contests. Problems are ordered from easiest to hardest.')

# map codeforces handles -> site usernames, so students see familiar names
di = db_handler.load_db()
handle_to_user = {}
for username, info in di.get('usernames', {}).items():
    handle = (info.get('cf_handle') or '').strip().lower()
    if handle:
        handle_to_user[handle] = username

if not data.contests:
    st.info('No contests have been configured yet.')
    st.stop()

tabs = st.tabs(['Overall'] + [c['name'] for c in data.contests])
overall_tab, contest_tabs = tabs[0], tabs[1:]

per_contest_tables = []
per_contest_names = []
for number, (tab, contest) in enumerate(zip(contest_tabs, data.contests), start=1):
    with tab:
        problems, rows = merge_contests(contest['ids'])
        if not problems:
            st.warning('Standings are unavailable for this contest right now.')
            continue

        table = build_table(problems, rows, contest.get('order'), handle_to_user, contest_number=number,
                            exclude=contest.get('exclude', ()))
        per_contest_tables.append(table)
        per_contest_names.append(contest['name'])
        st.caption(f"{len(table)} participants  ·  ranked by solved, then hardest problems solved, then penalty  ·  "
                   "✅ solved (-n = wrong tries before solving)  ·  ❌ (n) = n wrong tries, unsolved")
        st.dataframe(table.drop(columns=['_toughness']), hide_index=True, use_container_width=True)

        problem_columns = [c for c in table.columns if c not in ('Rank', 'Name', 'Solved', 'Penalty', '_toughness')]
        base = f"https://codeforces.com/group/tFKJNXEFZv/contest/{contest['ids'][0]}"
        st.markdown('Problems (easiest → hardest): '
                    + ' · '.join(f"[{p}]({base}/problem/{p})" for p in problem_columns))

        links = ' · '.join(
            f"[contest {i + 1}](https://codeforces.com/group/tFKJNXEFZv/contest/{cid})" if len(contest['ids']) > 1
            else f"[open on Codeforces](https://codeforces.com/group/tFKJNXEFZv/contest/{cid})"
            for i, cid in enumerate(contest['ids'])
        )
        st.markdown(links)

with overall_tab:
    if not per_contest_tables:
        st.warning('Standings are unavailable right now.')
    else:
        overall = build_overall_table(per_contest_tables, per_contest_names)
        st.caption(f"{len(overall)} participants  ·  ranked by total solved, then hardest problems solved "
                   "(by position in the contest; later contests count as harder), then total penalty  ·  "
                   "per-contest cells show solved (penalty), — = did not participate")
        st.dataframe(overall, hide_index=True, use_container_width=True)
