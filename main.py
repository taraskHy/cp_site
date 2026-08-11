import streamlit as st
import pickle
from pathlib import Path
import streamlit_authenticator as stauth
import parser
from parser import Status
import json
from pptx import Presentation
from io import BytesIO
from streamlit_star_rating import st_star_rating
from data import *
from codeforces_parser import fetch_user
import db_handler


st.set_page_config(page_title="Competitive Programming At University of Haifa", page_icon=":shark:", layout="wide")


di = db_handler.load_db()

authenticator = stauth.Authenticate(di, 'cpwebsite', '12345', 3)

# If a logged-in session points at an account that no longer exists
# (renamed or deleted), log it out cleanly instead of crashing.
if st.session_state.get('authentication_status') and \
        st.session_state.get('username') not in di['usernames']:
    authenticator.logout(location='unrendered')
    st.session_state['reg'] = 1

if 'reg' not in st.session_state:
    st.session_state['reg'] = 1

if st.session_state['reg'] == 1:
    try:
        if st.button("Register"):
            st.session_state['reg'] = 0
            st.rerun()
        authenticator.login('main', clear_on_submit=True)
        if st.session_state['authentication_status']:
            st.session_state['reg'] = 2
            st.rerun()
    except Exception as e:
        st.error(e)
elif st.session_state['reg'] == 0:
    try:
        if st.button("Login"):
            st.session_state['reg'] = 1
            st.rerun()
        email, \
            username, \
            name = authenticator.register_user(password_hint=False)
        # fields={'First name':'CSES Username', 'Last name': 'CSES Handle (Go to your profile, its the numbers in the URL!)'})
        if email and username and name:
            st.success('User registered successfully')
            st.session_state['reg'] = 3
            st.session_state['authentication_status'] = True
            st.session_state['username'] = username
            di['usernames'][username]['score'] = 0
            db_handler.save_db(di)

            st.rerun()
    except Exception as e:
        st.error(e)
elif st.session_state['reg'] == 3:
    with st.form('cses_info'):
        cses_username = st.text_input('CSES Username', key='cses_username')
        di['usernames'][st.session_state.get('username')]['cses_username'] = cses_username
        cses_handle = st.text_input('CSES Handle (Go to your profile, it\'s the numbers in the URL!)',
                                    key='cses_handle')
        di['usernames'][st.session_state.get('username')]['cses_handle'] = cses_handle


        submitted = st.form_submit_button('Submit')
        if cses_username and cses_handle and submitted:
            st.session_state['reg'] = 2
            db_handler.save_db(di)
            st.rerun()




def show_stars(star_count, key, size):
    # star_count == -1 marks a practice problem worth 0 points
    if star_count < 0:
        st.caption('Practice - no points')
    else:
        st_star_rating("", 5, star_count, size, read_only=True, dark_theme=True, key=key)


def week(list_of_questions, list_of_locked, stars, tasks, totoff):
    global di
    su = 0
    l = list_of_questions
    l2 = list_of_locked
    lc, mc, rc = st.columns(3)
    stars_size = 20
    with lc:
        k = [st.link_button(f"Problem {totoff + i + 1}", _[1]) for i, _ in enumerate(l)]
        st.text('Finish these problems to unlock more challenging ones!')
        # print(tasks)
    with mc:
        k = [show_stars(stars[i], f's{totoff+i}', stars_size) for i, _ in enumerate(l)]
    with rc:
        st.write('Here you can see the status of your problems:')
        k = [st.badge(f'Problem {totoff + i + 1}', icon=":material/check:", color="green") if tasks.get(tid, Status.NAT) == Status.AC
             else st.badge(f'Problem {totoff + i + 1}', color='gray') if tasks.get(tid, Status.NAT) == Status.NAT else
        st.badge(f'Problem {totoff + i + 1}', icon=":material/close:", color="red")
             for i, (tid, link, md) in enumerate(l)]
        p = [tasks.get(tid, Status.NAT) for (tid, link, md) in l]
        for i in range(len(l)):
            di['usernames'][st.session_state.get('username')][str(totoff + i)] = p[i]
        su = p.count(Status.AC)
        # db_handler.save_db(di)
    if su == len(p):
        st.subheader('More challenging problems unlocked!')
        lc, mc, rc = st.columns(3)
        off = len(l)
        with lc:
            k = [st.link_button(f"Problem {totoff + i + 1 + off}", _[1]) for i, _ in enumerate(l2)]
        with mc:
            print(off, len(l2))
            k = [show_stars(stars[i+off], f's{totoff+i+off}', stars_size) for i, _ in enumerate(l2)]
        with rc:
            # st.write('Here you can mark the problems you have completed (they will be saved on your next visit):')
            k2 = [st.badge(f'Problem {totoff + i + off + 1}', icon=":material/check:", color="green") if tasks.get(tid, Status.NAT) == Status.AC
                  else st.badge(f'Problem {totoff + i + off + 1}', color='gray') if tasks.get(tid, Status.NAT) == Status.NAT else
            st.badge(f'Problem {totoff + i + off + 1}', icon=":material/close:", color="red")
                  for i, (tid, link, md) in enumerate(l2)]
            p = [tasks.get(tid, Status.NAT) for (tid, link, md) in l2]
            for i in range(len(l2)):
                di['usernames'][st.session_state.get('username')][str(totoff + i + off)] = p[i]
            su += p.count(Status.AC)
            # db_handler.save_db(di)
        if su == off + len(p):
            st.success('Congrats! That is all for this week!')
        st.subheader(f'So far you have completed {su}/{off + len(p)} problems from this subject!')
    else:
        st.subheader(f'So far you have completed {su}/{len(p)} problems from this subject!')
    return totoff + len(stars)


def reformat_tasks(tasks, cf):
    new_tasks = {}
    cf = cf if isinstance(cf, dict) else {}
    for key, value in tasks.items():
        new_tasks[str(key)+'s'] = value
    for key, value in cf.items():
        new_tasks[str(key)+'f'] = value
    return new_tasks

def return_parsing():
    username = st.session_state.get('username')
    cses_handle = di['usernames'][username].get('cses_handle')
    cf_handle = di['usernames'][username].get('cf_handle')
    tasks = parser.get_user_info(cses_handle)
    cf_tasks = fetch_user(total_cf, cf_handle)

    if cf_tasks == -1:
        st.warning(
            'Please fill in your Codeforces information '
            'in the Profile section!'
        )
        st.stop()

    # fetch_user already displayed st.error(...)
    if cf_tasks is None:
        st.stop()

    tasks = reformat_tasks(tasks, cf_tasks)
    return tasks, cses_handle


if st.session_state.get('authentication_status') and st.session_state.get('reg') == 2:
    with st.container():
        authenticator.logout('Logout', 'sidebar')


    def Homepage():
        with st.container():
            st.title("Competitive Programming At University of Haifa")
            st.write("Welcome to the Competitive Programming At University of Haifa website!")
            st.write("This website is designed to help students learn and practice competitive programming.")
            st.write("---")

        with st.container():
            if not st.session_state.get('authentication_status'):
                st.rerun()
            tasks, cses_handle = return_parsing()
            st.header("Week zero - The Basics")
            new_off = week(week0u, week0l, week0s, tasks, 0)
            st.write("---")

        with st.container():
            if not st.session_state.get('authentication_status'):
                st.rerun()
            tasks, cses_handle = return_parsing()
            st.header("Week one - Greedy & DP")
            st.subheader("Greedy:")
            new_off = week(week1u[:2],[],week1s[:2],tasks,new_off)
            st.subheader("DP:")
            new_off = week(week1u[2:], week1l, week1s[2:],tasks, new_off)
            st.write("---")

        with st.container():
            if not st.session_state.get('authentication_status'):
                st.rerun()
            st.header("Homework")
            new_off = week(week1hw, week1hwl, week1hws, tasks, new_off)
            st.write("---")

        with st.container():
            if not st.session_state.get('authentication_status'):
                st.rerun()
            st.header("Week two - Graphs")
            new_off = week(week2u, week2l, week2s, tasks, new_off)
            st.write("---")
            st.markdown(
                """
                <div style="
                    color: red;
                    font-size: 2rem;
                    font-weight: 600;
                    line-height: 1.2;
                    margin: 0.25rem 0 1rem 0;
                ">
                    Week Two - Homework Solution:
                </div>
                """,
                unsafe_allow_html=True
            )
            path = "S/Solutions/Week_2/main.cpp"
            bo = BytesIO(open(path, 'rb').read())
            st.download_button(label='week 2 - HW solution', data=bo.getvalue(), file_name='main.cpp',
                               mime='text/x-c++src', key='cpw11825650')
            st.write("---")


        with st.container():
            if not st.session_state.get('authentication_status'):
                st.rerun()
            st.header("Week three - Flows & Matching")
            new_off = week(week3u, week3l, week3s, tasks, new_off)
            st.write("---")
            st.markdown(
                """
                <div style="
                    color: red;
                    font-size: 2rem;
                    font-weight: 600;
                    line-height: 1.2;
                    margin: 0.25rem 0 1rem 0;
                ">
                    Week Three - Homework + Contest Solution:
                </div>
                """,
                unsafe_allow_html=True
            )
            path = "S/Solutions/Week_3/main.cpp"
            bo = BytesIO(open(path, 'rb').read())
            st.download_button(label='Week 3 - Homework + Contest Solution', data=bo.getvalue(), file_name='main.cpp',
                               mime='text/x-c++src', key='cpw11825655')
            st.write("---")

        with st.container():
            if not st.session_state.get('authentication_status'):
                st.rerun()
            st.header("Week four - Segment Trees & Range Queries")
            new_off = week(week4u, week4l, week4s, tasks, new_off)
            st.write("---")

        with st.container():
            if not st.session_state.get('authentication_status'):
                st.rerun()
            st.header("Week five")
            new_off = week(week5u, week5l, week5s, tasks, new_off)
            st.write("---")
        # with st.container():
        #     if not st.session_state.get('authentication_status'):
        #         st.rerun()
        #     tasks, cses_handle = return_parsing()
        #     st.header("Week one - greedy and dynamic programming")
        #     new_off = week(week1u, week1l, week1s, tasks, new_off)
        #     st.write("---")

            # with st.container():
            #     if not st.session_state.get('authentication_status'):
            #         st.rerun()
            #     tasks, cses_handle = return_parsing()
            #     st.write("---")
            #     st.header("Week One - Intro to CP, STL, Greedy Algorithms and Dynamic Programming")
            #     # st.write("Here is the presentation for this week:")
            #     # pr = Presentation('presentations/week1.pptx')
            #     # bo = BytesIO()
            #     # pr.save(bo)
            #     # st.download_button(label='Week 1 Presentation', data=bo.getvalue(), file_name='Competitive-Programming-week-1.pptx', key='cpw11819')
            #     st.write("""
            #             Week One – Introduction to Competitive Programming Concepts
            #             This week covers foundational problem-solving strategies using C++ STL,
            #             greedy algorithms, and basic dynamic programming. You'll practice sorting,
            #             prefix sums, greedy interval selection, and subset-sum variations using
            #             classic problems from the CSES Problem Set.
            #            """)
            #     new_off = week(week1u, week1l, week1s, tasks, 0)

            # with st.container():
            #     st.write("---")
            #     st.header("Week Two - Graph Algorithms")

            #     st.write("""
            #             This week dives into fundamental graph techniques used in contests and real-world
            #             applications. Topics include breadth-first search (BFS), depth-first search (DFS),
            #             topological sorting for DAGs, Dijkstra’s algorithm for shortest paths, and
            #             cycle detection. Problems are selected to build strong intuition for graph traversal.
            #            """)
            #     new_off = week(week2u, week2l, week2s, tasks, new_off)

            # with st.container():
            #     st.write("---")
            #     st.header("Week Three - Max Flow, Matching, Union Find and SCC")

            #     st.write("""
            #             This week focuses on advanced graph techniques used in network flow, connectivity,
            #             and component analysis. You'll implement algorithms like Edmonds-Karp for maximum flow,
            #             use Union-Find (Disjoint Set Union) for efficient connectivity queries, and apply
            #             Kosaraju’s or Tarjan’s algorithm to find strongly connected components (SCCs).
            #             These topics are essential for tackling harder graph problems in competitions.
            #     """)
            #     new_off = week(week3u, week3l, week3s, tasks, new_off)


            # with st.container():
            #     st.write("---")
            #     st.header("Week Four – Segment Trees, Fenwick Trees, and Range Queries")

            #     st.write("""
            #             This week explores advanced data structures for efficient range queries and updates.
            #             You will implement segment trees and Fenwick trees (Binary Indexed Trees), handle
            #             point and range updates, and solve problems involving prefix sums, order statistics,
            #             and dynamic data manipulation. These skills are key for optimizing solutions
            #             to meet strict time constraints in competitive programming.
            #     """)
            #     new_off = week(week4u, week4l, week4s, tasks, new_off)

            # with st.container():
            #     st.write("---")
            #     st.header("Week Five – Binary Lifting, Euler Tour and Math")

            #     st.write("""
            #             This week explores fundamental tree algorithms and number theory techniques.
            #             Topics include computing tree diameters, subtree queries, and handling ancestor
            #             queries efficiently with Lowest Common Ancestor (LCA). On the math side, you’ll
            #             practice modular exponentiation and its extensions for solving power-related problems.
            #             Locked challenges introduce more advanced applications such as dynamic parent queries
            #             and bracket sequence analysis.
            #     """)
            #     new_off = week(week5u, week5l, week5s, tasks, new_off)

            db_handler.save_db(di)

    #pg = st.navigation([Homepage, 'Leaderboard.py', 'Profile.py', 'Material.py'])
    pg = st.navigation([Homepage, 'Leaderboard.py', 'Profile.py', 'Material.py'])
    pg.run()