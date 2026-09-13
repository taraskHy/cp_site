"""
Homework grades - admins pick which problems of each week count for the grade.

Every graded problem in a week is worth the same, so the grade is linear:
solving all of them gives 100, solving none gives 0.

The picks live in the database blob under 'must_do', as
{block name: [problem id, ...]}. Problem ids ('1068s', '4Af') are used instead
of positions because adding a problem to an early week shifts every position
after it, which would silently re-point the picks at the wrong problems.
"""
import data

# Same order as data.total_prob, which is also the order the homepage renders
# them in - that is what makes the positions below line up with the per-problem
# keys ('0', '1', ...) stored on each user record.
BLOCKS = [
    ('Week zero - The Basics', 'Week 0', data.week0u, data.week0l),
    ('Week one - Greedy & DP', 'Week 1', data.week1u, data.week1l),
    ('Week one - Homework', 'HW 1', data.week1hw, data.week1hwl),
    ('Week two - Graphs', 'Week 2', data.week2u, data.week2l),
    ('Week three - Flows & Matching', 'Week 3', data.week3u, data.week3l),
    ('Week four - Segment Trees & Range Queries', 'Week 4', data.week4u, data.week4l),
    ('Week five', 'Week 5', data.week5u, data.week5l),
    ('Week six', 'Week 6', data.week6u, data.week6l),
    ('Final prep 1 - Greedy & DP', 'Prep 1', data.prep1u, []),
    ('Final prep 2 - Graphs, SCC & Union-Find', 'Prep 2', data.prep2u, []),
    ('Final prep 3 - Flows & Matching', 'Prep 3', data.prep3u, []),
    ('Final prep 4 - Segment Trees & Decomposition', 'Prep 4', data.prep4u, []),
    ('Final prep 5 - Trees, Euler tour & Binary lifting', 'Prep 5', data.prep5u, []),
    ('Final prep 6 - Math', 'Prep 6', data.prep6u, []),
    ('Final prep 7 - Game Theory', 'Prep 7', data.prep7u, []),
]

AC = 0  # parser.Status.AC, repeated here so this module stays import-light

position_by_id = {problem[0]: i for i, problem in enumerate(data.total_prob)}


def blocks():
    """[(name, short name, [(position, problem), ...]), ...] for every week."""
    out = []
    offset = 0
    for name, short, unlocked, locked in BLOCKS:
        items = [(offset + i, problem) for i, problem in enumerate(list(unlocked) + list(locked))]
        offset += len(items)
        out.append((name, short, items))
    return out


def load_must(di):
    """{block name: [problem id, ...]} - the problems that count for a grade."""
    return di.get('must_do') or {}


def graded_positions(di):
    """Positions of every graded problem, for marking them on the homepage."""
    out = set()
    for ids in load_must(di).values():
        out.update(position_by_id[pid] for pid in ids if pid in position_by_id)
    return out


def grade(user_record, must_ids):
    """(solved, total, grade out of 100). grade is None when nothing is graded."""
    ids = [pid for pid in must_ids if pid in position_by_id]
    if not ids:
        return 0, 0, None
    solved = sum(1 for pid in ids if user_record.get(str(position_by_id[pid]), 2) == AC)
    return solved, len(ids), round(100 * solved / len(ids))


def block_grade(di, username, block_name):
    """The grade one student has in one week."""
    record = di.get('usernames', {}).get(username) or {}
    return grade(record, load_must(di).get(block_name, []))


def problem_label(position):
    """'Problem 12 - CSES 1068', matching the numbering used on the homepage."""
    pid, url, kind = data.total_prob[position]
    return f"Problem {position + 1} - {'CSES' if kind == 's' else 'CF'} {pid[:-1]}"
