import streamlit as st
from pathlib import Path
import pickle
import pandas as pd
import db_handler

import data

admins = data.admins

st.title("Our Notebook (Partially)")
st.header("Here you will find the algorithm implementations for things we believe you need.")

t = st.tabs(["Dinic", "SCC", "Union Find", "MCMF", "Hungarian", "Seg Tree", "Lazy Seg Tree", "Fenwick",
             "Binary Lifting"
             ])

with t[0]:
    st.markdown("""
    Description: Flow algorithm with complexity $O(V E \\log U)$ where $U = \\mathrm{max} |cap|$.  
    
    Time Complexity: $O(\\mathrm{min}(E^{\\frac{1}{2}}, V^{\\frac{2}{3}})E)$   if $U = 1$.
    
    $O(\\sqrt V E)$ for bipartite matching.""")


    code = """
    #define sz(yarin) ((int)(yarin).size())
    
    struct Dinic {
        struct Edge {
            int to, rev;
            ll c, oc;
            ll flow() { return max(oc - c, 0LL); } // if you need flows
        };
        vi lvl, ptr, q;
        vector<vector<Edge>> adj;
        Dinic(int n) : lvl(n), ptr(n), q(n), adj(n) {}
        void addEdge(int a, int b, ll c, ll rcap = 0) {
            adj[a].push_back({b, sz(adj[b]), c, c});
            adj[b].push_back({a, sz(adj[a]) - 1, rcap, rcap});
        }
        ll dfs(int v, int t, ll f) {
            if (v == t || !f) return f;
            for (int& i = ptr[v]; i < sz(adj[v]); i++) {
                Edge& e = adj[v][i];
                if (lvl[e.to] == lvl[v] + 1)
                    if (ll p = dfs(e.to, t, min(f, e.c))) {
                        e.c -= p, adj[e.to][e.rev].c += p;
                        return p;
                    }
            }
            return 0;
        }
        ll calc(int s, int t) {
            ll flow = 0; q[0] = s;
            rep(L,0,31) do { // ’ int L=30’ maybe faster for random data
                    lvl = ptr = vi(sz(q));
                    int qi = 0, qe = lvl[s] = 1;
                    while (qi < qe && !lvl[t]) {
                        int v = q[qi++];
                        for (Edge e : adj[v])
                            if (!lvl[e.to] && e.c >> (30 - L))
                                q[qe++] = e.to, lvl[e.to] = lvl[v] + 1;
                    }
                    while (ll p = dfs(s, t, LLONG_MAX)) flow += p;
                } while (lvl[t]);
            return flow;
        }
        bool leftOfMinCut(int a) { return lvl[a] != 0; }
    };
    
    """
    st.code(code, language='cpp', line_numbers=True)

with t[1]:
    st.markdown('''
    Description: Finds strongly connected components of a directed graph. Visits/indexes SCCs in topological order. 
    
    Time: $O(|V| + |E|)$ Usage: scc(graph) returns an array that has the ID of each node's SCC.''')

    code = """
    
    namespace SCCKosaraju {
        vector<vector<int>> adj, radj;
        vector<int> todo, comp;
        vector<bool> vis;
        void dfs1(int x) {
            vis[x] = 1;
            for (int y : adj[x])
                if (!vis[y]) dfs1(y);
            todo.push_back(x);
        }
        void dfs2(int x, int i) {
            comp[x] = i;
            for (int y : radj[x])
                if (comp[y] == -1) dfs2(y, i);
        }
        vector<int> scc(vector<vector<int>>& _adj) {
            adj = _adj;
            int time = 0, n = adj.size();
            comp.resize(n, -1), radj.resize(n), vis.resize(n);
            for (int x = 0; x < n; x++)
                for (int y : adj[x]) radj[y].push_back(x);
            for (int x = 0; x < n; x++)
                if (!vis[x]) dfs1(x);
            reverse(todo.begin(), todo.end());
            for (int x : todo)
                if (comp[x] == -1) dfs2(x, time++);
            return comp;
        }
    }; // namespace SCCKosaraju
    
    """
    st.code(code, language='cpp', line_numbers=True)

with t[2]:


    st.markdown('''
    Description: The regular Union Find from class.
    
    Time: $O(\\alpha (n))$ for both operations. 
    
    Run pre() before usage.''')

    code = """
    
    const int maxn =2e5+5;
    int p[maxn], s[maxn];
    void pre(){
        rep(i, 0, maxn) p[i] = i, s[i] = 1;
    }
    int find(int a) { return a == p[a] ? a : p[a] = find(p[a]); }
    int onion(int a, int b) {
        if((a = find(a)) == (b = find(b))) return 0;
        if(s[a] < s[b]) swap(a, b);
        return s[a] += s[b], p[b] = a, 1;
    }
    
    """
    st.code(code, language='cpp', line_numbers=True)

with t[3]:
    st.markdown('''
    Description: Min-cost max-flow. cap[i][j] != cap[j][i] is allowed; double edges are not.
    
    If costs can be negative, call setpi before maxflow, but note that negative cost cycles are not supported.
    
    To obtain the actual flow, look at positive values only.
    
    Time: Approximately $O(E^2)$''')

    code = """
    #include <ext/pb_ds/priority_queue.hpp> // remove if not working...
    const ll INF = numeric_limits<ll>::max() / 4;
    struct MCMF {
      struct edge { int from, to, rev; ll cap, cost, flow; };
      int N;
      vector<vector<edge>> ed;
      vi seen;
      vector<ll> dist, pi;
      vector<edge*> par;
      MCMF(int N): N(N), ed(N), seen(N), dist(N), pi(N), par(N) {}
      void addEdge(int from, int to, ll cap, ll cost) {
        if (from == to) return;
        ed[from].push_back(edge{from, to, sz(ed[to]), cap, cost,
                                0});
        ed[to].push_back(edge{to, from, sz(ed[from]) - 1, 0,
                              -cost, 0}); }
      void path(int s) {
        fill(all(seen), 0);
        fill(all(dist), INF);
        dist[s] = 0;
        ll di;
        __gnu_pbds::priority_queue<pair<ll, int>> q;
        vector<decltype(q)::point_iterator> its(N);
        q.push({0, s});
        while (!q.empty()) {
          s = q.top().second;
          q.pop();
          seen[s] = 1;
          di = dist[s] + pi[s];
          for (edge& e : ed[s]) if (!seen[e.to]) {
            ll val = di - pi[e.to] + e.cost;
            if (e.cap - e.flow > 0 && val < dist[e.to]) {
              dist[e.to] = val;
              par[e.to] = &e;
              if (its[e.to] == q.end())
                its[e.to] = q.push({-dist[e.to], e.to});
              else q.modify(its[e.to], {-dist[e.to], e.to}); } } }
        rep (i, 0, N) pi[i] = min(pi[i] + dist[i], INF); }
      pair<ll, ll> maxflow(int s, int t) {
        ll totflow = 0, totcost = 0;
        while (path(s), seen[t]) {
          ll fl = INF;
          for (edge* x = par[t]; x; x = par[x->from])
            fl = min(fl, x->cap - x->flow);
          totflow += fl;
          for (edge* x = par[t]; x; x = par[x->from]) {
            x->flow += fl;
            ed[x->to][x->rev].flow -= fl; } }
        rep (i, 0, N) for (edge& e : ed[i])
          totcost += e.cost * e.flow;
        return {totflow, totcost / 2}; }
      // If some costs can be negative, call this before maxflow:
      void setpi(int s) { // (otherwise, leave this out)
        fill(all(pi), INF);
        pi[s] = 0;
        int it = N, ch = 1;
        ll v;
        while (ch-- && it--) rep (i, 0, N) if (pi[i] != INF)
          for (edge& e : ed[i]) if (e.cap)
            if ((v = pi[i] + e.cost) < pi[e.to])
              pi[e.to] = v, ch = 1;
        assert(it >= 0); // negative cost cycle
      } };
    
    """
    st.code(code, language='cpp', line_numbers=True)

with t[4]:

    st.markdown('''
    Description: Given a weighted bipartite graph, matches every node on
    the left with a node on the right such that no nodes are in two matchings and the sum of the edge weights is minimal. 
    
    Takes cost[N][M], where cost[i][j] = cost for L[i] to be matched with R[j] and
    returns (min cost, match), where L[i] is matched with R[match[i]]. 
    
    Negate costs for max cost. Requires $N \\le M$.
    
    Time: $O(N^2\\cdot M)$''')

    code = """
    pair<int, vi> hungarian(const vector<vi>& a) {
      if (a.empty()) return {0, {}};
      int n = sz(a) + 1, m = sz(a[0]) + 1;
      vi u(n), v(m), p(m), ans(n - 1);
      rep (i, 1, n) {
        p[0] = i;
        int j0 = 0; // add "dummy" worker 0
        vi dist(m, INT_MAX), pre(m, -1);
        vector<bool> done(m + 1);
        do { // dijkstra
          done[j0] = true;
          int i0 = p[j0], j1, delta = INT_MAX;
          rep (j, 1, m) if (!done[j]) {
            auto cur = a[i0 - 1][j - 1] - u[i0] - v[j];
            if (cur < dist[j]) dist[j] = cur, pre[j] = j0;
            if (dist[j] < delta) delta = dist[j], j1 = j; }
          rep (j, 0, m) {
            if (done[j]) u[p[j]] += delta, v[j] -= delta;
            else dist[j] -= delta; }
          j0 = j1; } while (p[j0]);
        while (j0) { // update alternating path
          int j1 = pre[j0];
          p[j0] = p[j1], j0 = j1; } }
      rep (j, 1, m) if (p[j]) ans[p[j] - 1] = j - 1;
      return {-v[0], ans}; // min cost
    }
    """
    st.code(code, language='cpp', line_numbers=True)

with t[5]:
    st.markdown('''
    This is the implementation of Segment Tree, using arrays.
    
    This version supports the following:
    
    Build ($n$) : $O(n)$
    
    Query sum $[l, r]$: $O(\\log n)$
    
    Update add to $A_i$ another $k$ : $O(\\log n)$
    
    ''')

    code = """
    struct segment_tree_arr{
        int n;
        vi value;
        segment_tree_arr(int n): value(4*n, 0), n(n){
            build(1, 1, n);
        }
    
        void build(int node, int l, int r){
            if(l == r) return;
            int m = (l+r) / 2;
            build(2*node, l, m), build(2*node + 1, m+1, r);
            value[node] = value[2*node] + value[2*node + 1];
        }
    
        inline void update(int i, int k) { _update(1, i, k, 1, n); }
        void _update(int node, int i, int k, int l, int r){
            value[node] += k;
            if(l == r) return;
            int m = (l+r)/2;
            return (m < i) ? _update(2*node + 1, i, k, m+1, r) : _update(2*node, i, k, l, m);
        }
        inline int query(int l, int r) { return _query(1, l, r, 1, n); }
        int _query(int node, int ql, int qr, int l, int r){
            if(r < ql || qr < l) return 0;
            if(ql <= l && r <= qr) return value[node];
            int m = (l+r) / 2;
            return _query(2*node, ql, qr, l, m) + _query(2*node+1, ql, qr, m+1, r);
        }
    };
    """
    st.code(code, language='cpp', line_numbers=True)

with t[6]:
    st.markdown('''
    This is the implementation of Lazy Segment Tree, using arrays.
    
    This version supports the following:
    
    Build ($n$) : $O(n)$
    
    Query sum $[l, r]$: $O(\\log n)$
    
    Update add to range $[l, r]$ another $k$ : $O(\\log n)$
    ''')

    code = """
    struct lazy_segment_tree_arr{
        int n;
        vi value, lazy;
        lazy_segment_tree_arr(int n): value(4*n, 0), lazy(4*n, 0), n(n){
            build(1, 1, n);
        }
    
        void build(int node, int l, int r){
            if(l == r) return;
            int m = (l+r) / 2;
            build(2*node, l, m), build(2*node + 1, m+1, r);
            pullup(node);
        }
    
        inline void pullup(int node){ value[node] = value[2*node] + value[2*node + 1]; }
        inline void pushdown(int node, int l, int m, int r){
            int lz = lazy[node];
            value[2*node]   += lz * (m-l+1);
            value[2*node+1] += lz * (r-m);
            lazy[2*node]    += lz;
            lazy[2*node+1]  += lz;
            lazy[node] = 0;
        }
    
        inline void update(int l, int r, int k) { _update(1, l, r, k, 1, n); }
        void _update(int node, int ql, int qr, int k, int l, int r){
            if(r < ql || qr < l) return;
            if(ql <= l && r <= qr) {
                value[node] += k*(r-l+1); // addition to sum
                lazy[node] += k; // write down we had a change
                return;
            } int m = (l+r)/2;
            pushdown(node, l, m, r);
            _update(2*node, ql, qr, k, l, m), _update(2*node+1, ql, qr, k, m+1, r);
            pullup(node);
        }
        inline int query(int l, int r) { return _query(1, l, r, 1, n); }
        int _query(int node, int ql, int qr, int l, int r){
            if(r < ql || qr < l) return 0;
            if(ql <= l && r <= qr) return value[node];
            int m = (l+r) / 2;
            pushdown(node, l, m, r);
            return _query(2*node, ql, qr, l, m) + _query(2*node+1, ql, qr, m+1, r);
        }
    };


    """
    st.code(code, language='cpp', line_numbers=True)

with t[7]:
    st.markdown('''
    Description: Query $[0, i)$ and $[l, r)$ sums, and point updates.
    
    kth() returns the smallest index i s.t. query($i$) $> k$.
    
    Right endpoint is exclusive.
    
    Time: $O(\\log n)$ for all ops.''')

    code = """

    template<class T> struct Fenwick {
        vector<T> s;
        int n;
        Fenwick(int n): s(n + 1), n(n) {}
        void update(int i, T v) {
            for (i++; i <= n; i += i & -i) s[i] += v; 
        }
        T query(int i) { // EXCLUSIVE interval [0, i)
            T ans = 0;
            for (; i; i -= i & -i) ans += s[i];
            return ans; 
        } // INCLUSIVE-EXCLUSIVE interval [l, r)
           
        T query(int l, int r) { return query(r) - query(l); }
        
        int kth(T k) { // 0-indexed, returns n if k > sum of tree
            if (k < 0) return -1;
            int i = 0;
            for (int pw = 1 << __lg(n); pw; pw >>= 1)
                if (i + pw <= n && s[i + pw] <= k) 
                    k -= s[i += pw];
            return i; 
        } 
    };
    """
    st.code(code, language='cpp', line_numbers=True)

with t[8]:
    st.markdown('''
    Description: Calculate jumps up a tree, to support fast upward jumps and LCAs.
    
    Assumes the root node points to itself.
    
    Time: construction $O(N)$, queries $O(\\log N)$''')

    code = """

    struct lift {
          vi d, p, j;
          lift(vector<vi>& adj): d(sz(adj)), p(d), j(d) {
                dfs(0, adj); }
          void dfs(int u, vector<vi>& adj) {
                int jmp = (d[u] + d[j[j[u]]] == 2 * d[j[u]]) ? j[j[u]]
                                                         : u;
                for (int v : adj[u]) if (v != p[u])
                    d[v] = d[p[v] = u] + 1, j[v] = jmp, dfs(v, adj); }
          int lca(int u, int v) {
                if (d[u] < d[v]) swap(u, v);
                while (d[u] > d[v]) u = d[j[u]] >= d[v] ? j[u] : p[u];
                if (u == v) return u;
                while (p[u] != p[v]) if (j[u] != j[v]) u = j[u], v = j[v];
                else u = p[u], v = p[v];
                return p[u]; }
          int kth(int u, int k) {
                if (k > d[u]) return -1;
                k = d[u] - k;
                while (d[u] > k) u = d[j[u]] >= k ? j[u] : p[u];
                return u; 
          } 
    };
    """
    st.code(code, language='cpp', line_numbers=True)


