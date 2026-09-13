import pandas as pd
import streamlit as st

import data
import db_handler
import grading

st.header('Homework Grades')

username = st.session_state.get('username')
if username not in data.admins:
    st.error('This page is for admins only.')
    st.stop()

di = db_handler.load_db()
must = grading.load_must(di)
blocks = grading.blocks()

st.write('Tick the problems students have to solve. Every ticked problem in a week is worth the '
         'same, so solving all of them gives 100 and solving none gives 0. Students see which '
         'problems are graded and their own grade on the homepage, but cannot change them.')

st.subheader('Graded problems')
picked = {}
for name, short, items in blocks:
    already = set(must.get(name, []))
    with st.expander(f"{name}  -  {len(already)} of {len(items)} graded", expanded=False):
        columns = st.columns(3)
        chosen = []
        for offset, (position, problem) in enumerate(items):
            with columns[offset % 3]:
                if st.checkbox(grading.problem_label(position), value=problem[0] in already,
                               key=f'must_{position}'):
                    chosen.append(problem[0])
        picked[name] = chosen

if st.button('Save graded problems', type='primary'):
    # reload first, so saving the picks cannot roll back progress students
    # wrote while this page was open
    fresh = db_handler.load_db()
    fresh['must_do'] = {name: ids for name, ids in picked.items() if ids}
    db_handler.save_db(fresh)
    st.success('Saved.')
    st.rerun()

st.write('---')
st.subheader('Grades')

graded_blocks = [(name, short) for name, short, _ in blocks if must.get(name)]
if not graded_blocks:
    st.info('No problems are graded yet - tick some above and save.')
    st.stop()

show_admins = st.checkbox('Show admins', value=False)
rows = []
for user, record in di.get('usernames', {}).items():
    if user in data.black:
        continue
    if user in data.admins and not show_admins:
        continue
    row = {'Student': user}
    grades = []
    for name, short in graded_blocks:
        solved, total, value = grading.grade(record, must.get(name, []))
        row[short] = value
        grades.append(value)
    row['Average'] = round(sum(grades) / len(grades))
    rows.append(row)

if not rows:
    st.info('No students to show.')
    st.stop()

table = pd.DataFrame(rows).sort_values('Average', ascending=False)
st.caption(f"{len(table)} students  ·  every week is graded out of 100  ·  "
           'Average is the mean of the graded weeks')
st.dataframe(table, hide_index=True, use_container_width=True)
st.caption('Graded problems per week: '
           + '  ·  '.join(f"{short} ({len(must.get(name, []))})" for name, short in graded_blocks))
