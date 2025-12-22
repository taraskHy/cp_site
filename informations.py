import json
import db_handler

data = db_handler.load_db()
if data:
    i = data.get('usernames', {})
    l = [[user.get(str(c), 2) for c in range(5)] + [struser, user.get('etgar')] for struser, user in i.items()]
    for user in l:
        c = user.count(0)
        if c > 0:
            print(user)
all_presentations = [
    {
        "title": "Week 1: C++ STL",
        "pdf_path": "presentations/1-CPP+STL/cpp+stl.pptx.pdf"
    },
    {
        "title": "Week 2: Greedy Algorithms",
        "pdf_path": "presentations/2-GREEDY/greedy.pptx.pdf"
    },
    {
        "title": "Week 3: Dynamic Programming (DP)",
        "pdf_path": "presentations/3-DP/dp.pptx.pdf"
    },
    {
        "title": "Week 4: Graph Algorithms",
        "pdf_path": "presentations/4-GRAPH/graphs.pdf"
    },
    {
        "title": "Week 4: SCC & Union Find",
        "pdf_path": "presentations/4-GRAPH/scc+uf.pdf"
    },
    {
        "title": "Week 5: Flow & Matching",
        "pdf_path": "presentations/5-FLOW+MATCHING/flow+matching.pdf"
    },
    {
        "title": "Week 5: Min Cost Max Flow",
        "pdf_path": "presentations/5-FLOW+MATCHING/min_cost_max_flow.pdf"
    },
    {
        "title": "Week 6: Range Queries",
        "pdf_path": "presentations/6-RANGE_QUERIES/ragne_queries.pdf"
    },
    {
        "title": "Week 7: Trees",
        "pdf_path": "presentations/7-TREES/trees.pdf"
    },
    {
        "title": "Week 7: Binary Lifting",
        "pdf_path": "presentations/7-TREES/binary_lifting.pdf"
    },
    {
        "title": "Week 8: Math",
        "pdf_path": "presentations/8-MATH/math.pdf"
    }
]
