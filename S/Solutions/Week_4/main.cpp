#include <bits/stdc++.h>
#include <ext/pb_ds/priority_queue.hpp> // remove if not working...
using namespace  std;
#define ll long long
#define int long long
#define rep(a,b,c) for(int a=b;a<c;a++)
#define all(x) (x).begin(),(x).end()
#define vi vector<int>
#define pii pair<int,int>
#define vvi vector<vi>
#define vii vector<pii>
#define pb push_back
#define sz(x) ((int)(x).size())
#define vvii vector<vii>
vi a;
string st;
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

struct segment_tree_arr{
    int n;
    vi value;
    segment_tree_arr(int n): value(4*n, 0), n(n){
        build(1, 1, n);
    }

    void build(int node, int l, int r){
        if(l == r) {
            value[node] = a[l-1];
            return;
        }
        int m = (l+r) / 2;
        build(2*node, l, m), build(2*node + 1, m+1, r);
        value[node] = value[2*node] + value[2*node + 1];
    }

    inline void update(int i, int k) { _update(1, i, k, 1, n); }
    void _update(int node, int i, int k, int l, int r){
        value[node] +=k;
        if(l == r) {
            return;
        }
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
struct segment_tree_arr_GCD{
    int n;
    vi cnt;
    vi value;
    segment_tree_arr_GCD(int n): value(4*n, 0),cnt(4*n,0), n(n){
        build(1, 1, n);
    }

    void build(int node, int l, int r){
        if(l == r) {
            value[node] = a[l-1];
            cnt[node] = 1;
            return;
        }
        int m = (l+r) / 2;
        build(2*node, l, m), build(2*node + 1, m+1, r);
        value[node] = gcd( value[2*node],value[2*node + 1]);
        cnt[node] += (value[node]==value[2*node])?cnt[2*node]:0;
        cnt[node] += (value[node]==value[2*node+1])?cnt[2*node+1]:0;
    }

    inline pii query(int l, int r) { return _query(1, l, r, 1, n); }
    pii _query(int node, int ql, int qr, int l, int r){
        if(r < ql || qr < l) return {0,0};
        if(ql <= l && r <= qr) return {value[node],cnt[node]};
        int m = (l+r) / 2;
        pii lq = _query(2*node, ql, qr, l, m);
        pii rq = _query(2*node+1, ql, qr, m+1, r);
        pii ans = {0,0};
        ans.first = gcd(lq.first, rq.first);
        ans.second += (ans.first==lq.first)?lq.second:0;
        ans.second += (ans.first==rq.first)?rq.second:0;
        return ans;
    }
};
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
struct lazy_segment_tree_arr2{
    int n;
    vi value, lazy, lazyReset;
    lazy_segment_tree_arr2(int n): value(4*n, 0), lazy(4*n, 0), n(n), lazyReset(4*n) {
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
        if (lazyReset[node]) {
            value[2*node] = 0;
            value[2*node + 1] = 0;

            lazy[2*node] = 0;
            lazy[2*node+1] = 0;

            lazyReset[2*node] = 1;
            lazyReset[2*node+1] = 1;
            lazyReset[node] = 0;
        }
        int lz = lazy[node];
        value[2*node]   += lz * (m-l+1);
        value[2*node+1] += lz * (r-m);
        lazy[2*node]    += lz;
        lazy[2*node+1]  += lz;
        lazy[node] = 0;
    }

    inline void reset(int l, int r) {
        _reset(1, l, r, 1, n);
    }
    void _reset(int node, int ql, int qr, int l, int r) {
        if (r < ql || qr < l)
            return;
        if (ql <= l && r <= qr) {
            value[node] = 0;
            lazy[node] = 0;
            lazyReset[node] = 1;
            return;
        }
        int m = (l+r)/2;
        pushdown(node, l, m, r);
        _reset(2*node, ql, qr, l, m);
        _reset(2*node+1, ql, qr, m+1, r);

        pullup(node);
    }
    void writeString(char c, string& s) {
        writeString(1, 1, n, c, s);
    }
    void writeString(int node, int l, int r, char c, string& s) {
        if (value[node] == 0)
            return;
        if (l==r) {
            s[l-1]=c;
            return;
        }
        int m = (l+r)/2;
        pushdown(node, l, m, r);
        writeString(2 * node, l, m, c, s);
        writeString(2 * node + 1, m + 1, r, c, s);
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
struct lazy_segment_tree_arr3{
    int n;
    vi value, lazy, curr;
    lazy_segment_tree_arr3(int n): value(4*n, 0), lazy(4*n, 0), curr(4*n, 0), n(n){
        build(1, 1, n);
    }

    void build(int node, int l, int r){
        if(l == r) {
            value[node] = a[l];
            return;
        }
        int m = (l+r) / 2;
        build(2*node, l, m), build(2*node + 1, m+1, r);
        pullup(node);
    }

    inline void pullup(int node) {
        value[node] = value[2*node] + value[2*node + 1];
    }
    inline void pushdown(int node, int l, int m, int r){
        int lz = lazy[node];
        if(curr[node]) {
            value[2*node] = curr[node]*(m-l+1);
            lazy[2*node] = 0;
            curr[2*node] = curr[node];

            value[2*node+1] = curr[node]*(r-m);
            lazy[2*node+1] = 0;
            curr[2*node+1] = curr[node];
            curr[node] = 0;
        }

        if(lazy[node]) {
            int lz = lazy[node];
            value[2*node] += lz*(m-l+1);
            if(curr[2*node]) {
                curr[2*node] +=lz;
            }
            else {
                lazy[2*node] += lz;
            }

            value[2*node+1] += lz*(r-m);
            if(curr[2*node+1]) {
                curr[2*node+1] +=lz;
            }
            else {
                lazy[2*node+1] += lz;
            }
            lazy[node] = 0;
        }
    }

    inline void update(int l, int r, int k) { _update(1, l, r, k, 1, n); }
    void _update(int node, int ql, int qr, int k, int l, int r){
        if(r < ql || qr < l) return;
        if(ql <= l && r <= qr) {
            value[node] += k*(r-l+1);
            if (curr[node])
                curr[node] += k;
            else {
                lazy[node] += k;
            }
            return;
        } int m = (l+r)/2;
        pushdown(node, l, m, r);
        _update(2*node, ql, qr, k, l, m), _update(2*node+1, ql, qr, k, m+1, r);
        pullup(node);
    }

    inline void assign(int l, int r, int k) { _assign(1, l, r, k, 1, n); }
    void _assign(int node, int ql, int qr, int k, int l, int r){
        if(r < ql || qr < l) return;
        if(ql <= l && r <= qr) {
            value[node] = k*(r-l+1);
            lazy[node] = 0;
            curr[node] = k;
            return;
        } int m = (l+r)/2;
        pushdown(node, l, m, r);
        _assign(2*node, ql, qr, k, l, m), _assign(2*node+1, ql, qr, k, m+1, r);
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
struct lazy_segment_tree_arr4{
    int n;
    vi value, lazy;
    lazy_segment_tree_arr4(int n): value(4*n, 0), lazy(4*n, 0), n(n){
        build(1, 1, n);
    }

    void build(int node, int l, int r){
        if(l == r) {
            value[node] = a[l-1];
            return;
        }
        int m = (l+r) / 2;
        build(2*node, l, m), build(2*node + 1, m+1, r);
        pullup(node);
    }

    inline void pullup(int node){ value[node] = min(value[2*node], value[2*node + 1]); }
    inline void pushdown(int node, int l, int m, int r){
        int lz = lazy[node];
        value[2*node]   += lz;
        value[2*node+1] += lz;
        lazy[2*node]    += lz;
        lazy[2*node+1]  += lz;
        lazy[node] = 0;
    }

    inline void update(int l, int r, int k) { _update(1, l, r, k, 1, n); }
    void _update(int node, int ql, int qr, int k, int l, int r){
        if(r < ql || qr < l) return;
        if(ql <= l && r <= qr) {
            value[node] += k;
            lazy[node] += k;
            return;
        } int m = (l+r)/2;
        pushdown(node, l, m, r);
        _update(2*node, ql, qr, k, l, m), _update(2*node+1, ql, qr, k, m+1, r);
        pullup(node);
    }
    inline int query(int l, int r) { return _query(1, l, r, 1, n); }
    int _query(int node, int ql, int qr, int l, int r){
        if(r < ql || qr < l) return LONG_MAX/4;
        if(ql <= l && r <= qr) return value[node];
        int m = (l+r) / 2;
        pushdown(node, l, m, r);
        return min(_query(2*node, ql, qr, l, m) , _query(2*node+1, ql, qr, m+1, r));
    }
};
struct lazy_segment_tree_xor{
    int n;
    vector<long long> value;
    vi lazy;
    vector<array<int,21>> cnt;
    lazy_segment_tree_xor(int n): value(4*n,0),lazy(4*n,0),cnt(4*n),n(n){
        build(1,1,n);
    }

    void build(int node,int l,int r){
        if(l==r) {
            value[node] = a[l-1];
            for(int j=0;j<21;j++) {
                if(a[l-1]&(1<<j))
                    cnt[node][j] = 1;
            }
            return;
        }
        int m = (l+r)/2;
        build(2*node,l,m),build(2*node+1,m+1,r);
        pullup(node);
    }

    inline void pullup(int node){
        value[node] = value[2*node]+value[2*node+1];
        for(int j=0;j<21;j++) {
            cnt[node][j] = cnt[2*node][j]+cnt[2*node+1][j];
        }
    }

    void apply(int node,int l,int r,int x){
        for(int j=0;j<21;j++) {
            if(x&(1<<j)) {
                cnt[node][j] = (r-l+1)-cnt[node][j];
            }
        }
        value[node] = 0;
        for(int j=0;j<21;j++) {
            value[node] += 1LL*cnt[node][j]*(1LL<<j);
        }
        lazy[node] ^= x;
    }

    inline void pushdown(int node,int l,int m,int r){
        if(!lazy[node]) return;
        apply(2*node,l,m,lazy[node]);
        apply(2*node+1,m+1,r,lazy[node]);
        lazy[node] = 0;
    }

    inline void update(int l,int r,int x){
        _update(1,l,r,x,1,n);
    }

    void _update(int node,int ql,int qr,int x,int l,int r){
        if(r<ql||qr<l) return;
        if(ql<=l&&r<=qr) {
            apply(node,l,r,x);
            return;
        }
        int m = (l+r)/2;
        pushdown(node,l,m,r);
        _update(2*node,ql,qr,x,l,m);
        _update(2*node+1,ql,qr,x,m+1,r);
        pullup(node);
    }

    inline long long query(int l,int r){
        return _query(1,l,r,1,n);
    }

    long long _query(int node,int ql,int qr,int l,int r){
        if(r<ql||qr<l) return 0;
        if(ql<=l&&r<=qr)
            return value[node];
        int m = (l+r)/2;
        pushdown(node,l,m,r);

        return _query(2*node,ql,qr,l,m) + _query(2*node+1,ql,qr,m+1,r);
    }
};

struct segment_tree_arr2{
    int n;
    vi value, lvl;
    segment_tree_arr2(int n): value(4*n, 0), n(n) , lvl(4*n,0){
        build(1, 1, n);
    }

    void build(int node, int l, int r){
        if(l == r) {
            lvl[node] = 0;
            value[node] = a[l-1];
            return;
        }
        int m = (l+r) / 2;
        build(2*node, l, m), build(2*node + 1, m+1, r);
        lvl[node] = lvl[2*node+1] + 1;
        if(!(lvl[node]%2)) {
            value[node] = value[2*node] ^ value[2*node+1];
        }
        else {
            value[node] = value[2*node] | value[2*node+1];
        }
    }

    inline void update(int i, int k) { _update(1, i, k, 1, n); }
    void _update(int node, int i, int k, int l, int r){
        if(l == r) {
            value[node] = k;
            return;
        }
        int m = (l+r)/2;
        (m < i) ? _update(2*node + 1, i, k, m+1, r) : _update(2*node, i, k, l, m);
        if(!(lvl[node]%2)) {
            value[node] = value[2*node] ^ value[2*node+1];
        }
        else {
            value[node] = value[2*node] | value[2*node+1];
        }
    }
};
struct segment_tree_arr3{
    int n;
    vi value;
    vector<bitset<26>> cnt;
    segment_tree_arr3(int n): value(4*n, 0), n(n), cnt(4*n, bitset<26>(0)){
        build(1, 1, n);
    }

    void build(int node, int l, int r){
        if(l == r) {
            value[node] = 1;
            cnt[node].set(st[l-1]-'a');
            return;
        }
        int m = (l+r) / 2;
        build(2*node, l, m), build(2*node + 1, m+1, r);
        value[node] = 0;
        cnt[node] = cnt[node*2] | cnt[node*2+1];
        value[node] = cnt[node].count();
    }

    inline void update(int i, int k) { _update(1, i, k, 1, n); }
    void _update(int node, int i, int k, int l, int r){
        if(l == r) {
            value[node] = 1;
            cnt[node].reset();
            cnt[node].set(k);
            return;
        }
        int m = (l+r)/2;
        (m < i) ? _update(2*node + 1, i, k, m+1, r) : _update(2*node, i, k, l, m);
        value[node] = 0;
        cnt[node] = cnt[2*node]|cnt[2*node+1];
        value[node] = cnt[node].count();
    }
    inline int query(int l, int r) { return _query(1, l, r, 1, n).count(); }
    bitset<26> _query(int node, int ql, int qr, int l, int r){
        if(r < ql || qr < l) return 0;
        if(ql <= l && r <= qr) return cnt[node];
        int m = (l+r) / 2;
        return _query(2*node, ql, qr, l, m) | _query(2*node+1, ql, qr, m+1, r);
    }
};
struct segment_tree_arr4{
    int n;
    vi open;
    vi close;
    vi pairs;
    segment_tree_arr4(int n): open(4*n, 0), close(4*n, 0), pairs(4*n, 0), n(n){
        build(1, 1, n);
    }

    void build(int node, int l, int r){
        if(l == r) {
            if (st[l-1]=='(')
                open[node] = 1;
            else
                close[node] = 1;
            return;
        }
        int m = (l+r) / 2;
        build(2*node, l, m), build(2*node + 1, m+1, r);
        pairs[node] = pairs[node*2] + pairs[node*2+1] + min(open[2*node], close[2*node+1]);
        open[node] = open[2*node] + open[2*node + 1] - min(open[2*node], close[2*node+1]);
        close[node] = close[2*node] + close[2*node+1] - min(open[2*node], close[2*node+1]);
    }


    inline int query(int l, int r) { return 2*get<0>(_query(1, l, r, 1, n)); }
    tuple<int,int,int> _query(int node, int ql, int qr, int l, int r){
        if(r < ql || qr < l) return {0,0,0};
        if(ql <= l && r <= qr) return {pairs[node],open[node],close[node]};
        int m = (l+r) / 2;
        auto left =  _query(2*node, ql, qr, l, m);
        auto right = _query(2*node+1, ql, qr, m+1, r);
        return {get<0>(left) + get<0>(right) + min(get<1>(left), get<2>(right)),
            get<1>(left) + get<1>(right) - min(get<1>(left), get<2>(right)),
            get<2>(left) + get<2>(right) - min(get<1>(left), get<2>(right))};
    }
};

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
const int maxn =2e5;
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


//problems we didnt use:
/*void two_sort(){
    int n; cin >> n;
    int k; cin >> k;
    vi a(n);
    vi flag(n,0);
    rep(i,0,n) {
        cin >> a[i];
    }
    for (int i =0; i<n-1; i++) {
        if (a[i]<2*a[i+1]) {
            flag[i] = 1;
        }
    }
    int cnt = 0;
    int index = 0;
    int ans = 0;
    while (index < n) {
        if (flag[index]) {
            cnt++;
            if (cnt>=k) {
                ans ++;
            }
        }
        else {
            cnt = 0;
        }
        index++;
    }
    cout << ans << '\n';
}*/ // "2^sort"
/*void Sereja_and_Suffixes() {
    int n; cin >> n;
    int m; cin >> m;
    set<int> s;
    vi a(n);
    vi cnt(n);
    rep(i,0,n) {
        cin >> a[i];
    }
    for (int i = n-1; i >= 0; i--) {
        s.insert(a[i]);
        cnt[i] = s.size();
    }
    vi l(m);
    for (int i =0; i<m; i++) {
        int q; cin >> q;
        l[i] = q-1;
    }
    for (int i = 0; i<m; i++) {
        cout << cnt[l[i]] << '\n';
    }
}*/



void Dynamic_Range_Sum_Queries() {
    int n; cin >> n;
    int m; cin >> m;
    a.resize(n);
    rep(i,0,n) {
        cin >> a[i];
    }
    segment_tree_arr seg(n);
    vector<pair<int,pair<int,int>>> que(m);
    rep(i,0,m) {
        int q; cin >> q;
        int l, r; cin >> l >> r;
        que[i] = {q,{l,r}};
    }
    rep(i,0,m) {
        if(que[i].first == 1) {
            seg.update(que[i].second.first, que[i].second.second-a[que[i].second.first-1]);
        }
        else {
            cout << seg.query(que[i].second.first, que[i].second.second) << endl;
        }
    }
}
void Range_Updates_and_Sums() {
    int n,q;
    cin>>n>>q;
    a = vector<int>(n+1);
    for(int i=1;i<=n;i++) cin>>a[i];
    auto st = lazy_segment_tree_arr3(n);
    for(int i=0;i<q;i++) {
        int m,l,r,x;
        cin>>m;
        if (m==1) {
            cin>>l>>r>>x;
            st.update(l, r, x);
        }
        else if (m==2) {
            int b,c;
            cin>>l>>r>>x;
            st.assign(l, r, x);
        }
        else
        {
            cin>>l>>r;
            cout<<st.query(l, r)<<'\n';
        }
    }
}
void List_Removals() {
    int n;
    cin>>n;
    vector<int> v(n+1);
    vector<int> p(n+1);
    rep(i,1,n+1) cin>>v[i];
    rep(i,1,n+1) cin>>p[i];
    Fenwick<int> f(n);
    for (int i = 1; i <=n; i++) {
        f.update(i, 1);
    }
    for (int i = 1; i <= n; i++) {
        int idx = f.kth(p[i] - 1);
        cout << v[idx] << ' ';
        f.update(idx, -1);
    }
}
void Xenia_and_Bit_Operations() {
    int n,m;
    cin>>n>>m;
    a.resize(1LL<<n);
    rep(i,0,1<<n) cin>>a[i];
    segment_tree_arr2 seg((1LL<<n));
    for (int i = 0; i<m; i++) {
        int b,q;
        cin>>b>>q;
        seg.update(b,q);
        cout << seg.value[1]<<"\n";
    }

}
void Circular_RMQ() {
    int n;
    cin>>n;
    a.resize(n);
    rep(i,0,n) cin>>a[i];
    int m;
    lazy_segment_tree_arr4 seg(n);
    cin>>m;
    string line;
    getline(cin, line);
    for (int i =0; i<m; i++) {
        getline(cin,line); //old input format - not important
        stringstream ss(line);
        int l,r,v;
        ss>>l>>r;
        l++;
        r++;
        if (ss>>v){
            if (r<l) {
                seg.update(l, n, v);
                seg.update(1, r, v);
            }
            else {
                seg.update(l, r, v);
            }
        }
        else{
            if (r<l) {
                cout<<min(seg.query(l, n),seg.query(1, r))<<"\n";
            }
            else {
                cout<<seg.query(l, r)<<"\n";
            }
        }
    }
}
void Distinct_Characters_Queries() {
    cin>>st;
    int n = st.length();
    segment_tree_arr3 seg(n);

    int q;
    int num;
    cin>>num;
    for (int i = 0; i<num; i++) {
        cin>>q;
        if (q==1) {
            int pos;
            char c;
            cin>>pos>>c;
            seg.update(pos,c-'a');
        }
        else {
            int l,r;
            cin>>l>>r;
            cout << seg.query(l,r) << '\n';
        }
    }

}
void Sereja_and_Brackets() {
    cin>>st;
    segment_tree_arr4 seg(st.length());
    int m;
    cin>>m;
    for (int i =0; i<m; i++) {
        int l,r;
        cin>>l>>r;
        cout<<seg.query(l,r)<<'\n';
    }

}


void Ethan_and_the_Broken_Ranking_System() {
    int n,q;
    cin>>n>>q;
    vi low(n+1,1),high(n+1,n);
    rep(i,0,q) {
        int t,l,r,v;
        cin>>t>>l>>r>>v;
        if (t==1) {
            for (int j=l; j<=r; j++) {
                low[j] = max(low[j],v);
            }
        }
        else {
            for (int j=l; j<=r; j++) {
                high[j] = min(high[j],v);
            }
        }
    }

    rep(i,1,n+1) {
        if (low[i]>high[i]) {
            cout<<-1<<'\n';
            return;
        }
    }
    int source = 0;
    int sink = 2*n+1;
    MCMF mcmf(2*n+2);
    rep(i,1,n+1) {
        mcmf.addEdge(source,i,1,0);
    }
    rep(i,1,n+1) {
        for (int j=low[i]; j<=high[i]; j++) {
            mcmf.addEdge(i,n+j,1,0);
        }
    }
    rep(i,1,n+1) {
        rep(j,1,n+1) {
            mcmf.addEdge(n+i,sink,1,2*j-1);
        }
    }
    auto ans = mcmf.maxflow(source,sink);
    if (ans.first<n) {
        cout<<-1<<'\n';
        return;
    }
    cout<<ans.second<<'\n';
}
void Shalev_and_the_Suspicious_Gradebook() {
    int n; cin >> n;
    int q; cin >> q;
    vi a(n);
    vii query(q);
    rep(i,0,n) {
        cin >> a[i];
    }
    lazy_segment_tree_arr LS(n);
    rep(i,0,q) {
        cin >> query[i].first;
        cin >> query[i].second;
        LS.update(query[i].first,query[i].second,1);
    }
    set<pii> s;
    rep(i,0,n) {
        int f = LS.query(i+1,i+1);
        s.insert(make_pair(f,i));
    }
    sort(a.begin(),a.end());
    int cnt = 0;
    int sum = 0;
    for (auto i : s) {
        sum += a[cnt]*i.first;
        cnt++;
    }
    cout << sum << '\n';


}
void Ethans_Palindrome_Emergency() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    int n; cin >> n;
    int q; cin >> q;
    vector<lazy_segment_tree_arr2> arr(26, lazy_segment_tree_arr2(n));
    string s; cin >> s;
    rep(i,0,n) {
        arr[s[i]-'a'].update(i+1,i+1,1);
    }
    vii query(q);
    rep(i,0,q) {
        cin >> query[i].first;
        cin >> query[i].second;
    }
    for (auto i : query) {
        vi cnt(26);
        int odd = 0;
        int index = -1;
        int l = i.first;
        rep(j,0,26) {
            cnt[j] = (arr[j]).query(i.first,i.second);
            if (cnt[j] % 2) {
                odd++;
                index = j;
            }
        }
        if (odd>1) {
            continue;
        }
        rep(j,0,26) {
            arr[j].reset(i.first, i.second);
        }
        for (int j=0; j<26; j++) {
            int half = cnt[j]/2;
            if (half == 0)
                continue;
            int r = l + half - 1;
            arr[j].update(l, r, 1);
            l = r+1;
        }
        if (index != -1) {
            arr[index].update(l, l, 1);
            l++;
        }
        for (int j=25; j>=0; j--) {
            int half = cnt[j]/2;
            if (half == 0)
                continue;
            int r = l + half - 1;
            arr[j].update(l, r, 1);
            l = r+1;
        }
    }
    for (int i = 0; i<s.size(); i++) {
        rep(j,0,26) {
            if (arr[j].query(i+1,i+1)!=0) {
                s[i] = j + 'a';
                break;
            }
        }
    }
    cout << s << '\n';
}
void Shalevs_Divisibility_Talent_Show() {
    int n; cin >> n;
    a.resize(n);
    rep(i,0,n) {
        cin >> a[i];
    }
    segment_tree_arr_GCD sg(n);
    int t; cin >> t;
    rep(i,0,t) {
        int l,r;
        cin >> l >> r;
        cout << (r-l+1)-sg.query(l,r).second << '\n';
    }

}
void The_Glitched_Energy_Network() {
    int n;
    cin>>n;
    a.resize(n);
    rep(i,0,n) cin>>a[i];
    lazy_segment_tree_xor seg(n);
    int m;
    cin>>m;
    for(int i=0;i<m;i++) {
        int type;
        cin>>type;

        if(type==1) {
            int l,r;
            cin>>l>>r;
            cout<<seg.query(l,r)<<'\n';
        }
        else {
            int l,r,x;
            cin>>l>>r>>x;
            seg.update(l,r,x);
        }
    }
}
void Shalev_and_the_Questionable_Hiking_Trails() {
    int n; cin >> n;
    int m; cin >> m;
    vector<tuple<int,int,int>> edges(n-1);
    rep(i,0,n-1) {
        int u,v,w;
        cin >> u >> v >> w;
        edges[i] = {w, u,v};
    }
    sort(edges.begin(),edges.end());
    vii query(m);
    rep(i,0,m) {
        int q;
        cin >> q;
        query[i] = {q,i};
    }
    sort(query.begin(), query.end());
    vi ans(m);
    pre();

    int edgeindx = 0;
    int pairs = 0;

    for (auto i : query) {
        int q = i.first;
        int index = i.second;
        while (edgeindx<n-1&&get<0>(edges[edgeindx])<=q) {
            int u = get<1>(edges[edgeindx]);
            int v = get<2>(edges[edgeindx]);
            pairs+= (find(u)==find(v))? 0: s[find(u)]*s[find(v)];
            onion(u,v);
            edgeindx++;
        }
        ans[index] = pairs;
    }
    rep(i,0,m) {
        cout << ans[i] << " ";
    }

}
void Ethan_and_the_Extremely_Serious_Seating_Ritual() {
    int n; cin >> n;
    vi arr(n);
    priority_queue<tuple<int,int,int>> q;
    q.push({n,0,n-1});
    int i = 0;
    while (!q.empty()) {
        i++;
        auto p = q.top();
        q.pop();
        int l = -get<1>(p);
        int r = get<2>(p);
        if ((r-l+1)%2) {
            arr[(l+r)/2] = i;
            if (l-r!=0) {
                if (l <= (l+r)/2 - 1)
                    q.push({(l+r)/2 - l, -l, (l+r)/2 - 1});

                if ((l+r)/2 + 1 <= r)
                    q.push({r - (l+r)/2, -((l+r)/2 + 1), r});
            }
        }
        else {
            arr[(l+r-1)/2] = i;
            if (l-r!=0) {
                if (l <= (l+r-1)/2 - 1)
                    q.push({(l+r-1)/2 - l, -l, (l+r-1)/2 - 1});

                if ((l+r-1)/2 + 1 <= r)
                    q.push({r - (l+r-1)/2, -((l+r-1)/2 + 1), r});
            }
        }
    }
    for (auto i : arr) {
        cout << i << ' ';
    }
    cout << '\n';
}






signed main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int t = 1;
    //cin >> t;
    while (t--) {
        //the problem:
    }
    return 0;
}
