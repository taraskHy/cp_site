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
#define MOD 998244353LL

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
void Ethan_Please_Not_Again() {
    vi cnt(26);
    int n;
    cin>>n;
    vector<char> str(n);
    vi gain(n);
    for (int i = 0; i < n; i++) {
        cin>>str[i];
        cnt[str[i]-'a']++;
    }
    for (int i = 0; i < n; i++) {
        cin>>gain[i];
        gain[i]*=-1;
    }
    int letters = 0;
    int pairs = 26;
    int source = pairs + n/2;
    int sink = source+1;
    MCMF mcmf(sink+1);
    for (int i = 0; i < 26; i++) {
        if (cnt[i])
            mcmf.addEdge(source, i,cnt[i], 0);
    }
    for (int pair = 0; pair < n/2; pair++) {
        mcmf.addEdge(pair+pairs, sink, 2, 0);
    }
    for (int pair = 0; pair < n/2; pair++) {
        for (int i = 0; i < 26; i++) {
            int maxgain = 0;
            if (str[pair]-'a'==i) {
                maxgain = min(maxgain, gain[pair]);
            }
            if (str[n-1-pair]-'a'==i) {
                maxgain = min(maxgain, gain[n-1-pair]);
            }
            mcmf.addEdge(i, pair + pairs, 1, maxgain);
        }
    }
    mcmf.setpi(source);
    pii ans = mcmf.maxflow(source, sink);
    cout << -ans.second << endl;
}


signed D() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int t = 1;
    //cin >> t;
    while (t--) {
        Ethan_Please_Not_Again();
        cout<<"\n";
    }
    return 0;
}
