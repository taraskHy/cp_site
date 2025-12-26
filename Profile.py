import streamlit as st
from pathlib import Path
import pickle
import data
import db_handler


di = db_handler.load_db()

if st.session_state.get('username') in data.admins:
    if st.button('Print DB'):
        st.json(di)
    with st.form('copy user'):
        user = st.text_input('copy user:', key='cp')
        k = st.form_submit_button('submit')
        if k:
            if user in di['usernames']:
                st.session_state['username'] = user
                st.rerun()
    with st.form('fetch user'):
        user = st.text_input('fetch user:', key='fetch')
        k = st.form_submit_button('submit')
        if k:
            if user in di['usernames']:
                st.table(di['usernames'][user])

if st.button('Change CSES and Etgar info'):
    st.session_state['profcses'] = 1
if st.session_state.get('profcses', 0) == 1:
    with st.form('cses_info'):
        cses_username = st.text_input('CSES Username', key='cses_username')
        cses_handle = st.text_input('CSES Handle (Go to your profile, it\'s the numbers in the URL!)',
                                    key='cses_handle')
        etgar_num = st.text_input('What is the number of your ETGAR? (18/19/20) If you are not from Etgar, enter 0 :)',
                                  key='etgar')
        submitted = st.form_submit_button('Submit')
        if cses_username and cses_handle and submitted and etgar_num:
            st.session_state['profcses'] = 0
            di['usernames'][st.session_state.get('username')]['cses_handle'] = cses_handle
            di['usernames'][st.session_state.get('username')]['etgar'] = etgar_num
            di['usernames'][st.session_state.get('username')]['cses_username'] = cses_username

            db_handler.save_db(di)
            st.success('New Info Saved!')

if st.button('Change Codeforces info'):
    st.session_state['profcf'] = 1
if st.session_state.get('profcf', 0) == 1:
    with st.form('cf_info'):
        cf_handle = st.text_input('Codeforces Handle (Go to your profile, it\'s the username at codeforces)',
                                    key='cf_handle')
        submitted = st.form_submit_button('Submit')
        if cf_handle and submitted:
            st.session_state['profcf'] = 0
            di['usernames'][st.session_state.get('username')]['cf_handle'] = cf_handle

            db_handler.save_db(di)
            st.success('New Info Saved!')

if st.button('Change Username'):
    st.session_state['profus'] = 1
if st.session_state.get('profus', 0) == 1:
    with st.form('usinfo'):
        us = st.text_input('New Username', key='us')
        submitted = st.form_submit_button('Submit')
        if us and submitted:
            st.session_state['profus'] = 0
            entr = di['usernames'][st.session_state.get('username')]
            del di['usernames'][st.session_state.get('username')]
            di['usernames'][us] = entr

            db_handler.save_db(di)
            st.success('New Info Saved!')

