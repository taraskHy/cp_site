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
const int MOD=998244353;
const int MAXN=300005;
int power2[MAXN];
const long double COST_INF = 1e100L;
const ll INF = numeric_limits<ll>::max()/4;


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
struct double_MCMF {
struct edge { int from, to, rev; ll cap, flow; double cost; };
  int N;
  vector<vector<edge>> ed;
  vi seen;
  vector<double> dist, pi;
  vector<edge*> par;
  double_MCMF(int N): N(N), ed(N), seen(N), dist(N), pi(N), par(N) {}
  void addEdge(int from, int to, ll cap, double cost) {
    if (from == to) return;
    ed[from].push_back(edge{from, to, sz(ed[to]), cap, 0, cost});
    ed[to].push_back(edge{to, from, sz(ed[from]) - 1, 0, 0,
                          -cost}); }
  void path(int s) {
    fill(all(seen), 0);
    fill(all(dist), (double)INF);
    dist[s] = 0;
    double di;
    __gnu_pbds::priority_queue<pair<double, int>> q;
    vector<decltype(q)::point_iterator> its(N);
    q.push({0, s});
    while (!q.empty()) {
      s = q.top().second;
      q.pop();
      seen[s] = 1;
      di = dist[s] + pi[s];
      for (edge& e : ed[s]) if (!seen[e.to]) {
        double val = di - pi[e.to] + e.cost;
        if (e.cap - e.flow > 0 && val < dist[e.to]) {
          dist[e.to] = val;
          par[e.to] = &e;
          if (its[e.to] == q.end())
            its[e.to] = q.push({-dist[e.to], e.to});
          else q.modify(its[e.to], {-dist[e.to], e.to}); } } }
    rep (i, 0, N)
      pi[i] = min(pi[i] + dist[i], (double)INF); }
  pair<ll, double> maxflow(int s, int t) {
    ll totflow = 0;
    double totcost = 0;
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
  void setpi(int s) {
    fill(all(pi), (double)INF);
    pi[s] = 0;
    int it = N, ch = 1;
    double v;
    while (ch-- && it--) rep (i, 0, N) if (pi[i] != INF)
      for (edge& e : ed[i]) if (e.cap)
        if ((v = pi[i] + e.cost) < pi[e.to])
          pi[e.to] = v, ch = 1;
    assert(it >= 0);
  } };
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
int buildpath(int v,vector<vii>& u,int n, vi& path)
{
    if (v == n)
    {
        cout << path.size() << endl;
        for (int x : path)
        {
            cout << x << " ";
        }
        cout << endl;
        return 1;
    }
    while (!u[v].empty())
    {
        auto e = u[v].back();
        u[v].pop_back();
        path.pb(e.first);
        if (buildpath(e.first, u, n, path))
        {
            return 1;
        }
        path.pop_back();
    }
    return 0;
}
int buildpath2(int v,vector<vii>& u,int n, vi& path)
{
    if (v == n)
    {
        cout << path.size() << endl;
        for (int x : path)
        {
            cout << x+1 << " ";
        }
        cout << endl;
        return 1;
    }
    while (!u[v].empty())
    {
        auto e = u[v].back();
        u[v].pop_back();
        path.pb(e.first);
        if (buildpath2(e.first, u, n, path))
        {
            return 1;
        }
        path.pop_back();
    }
    return 0;
}


void Download_Speed() {
    int n,m;
    cin>>n>>m;
    Dinic d(n);
    rep(i,0,m)
    {
        int a,b,c;
        cin>>a>>b>>c;
        a--;
        b--;
        d.addEdge(a,b,c);
    }
    cout<<d.calc(0,n-1)<<'\n';
}
void School_Dance() {
    int n, m, k;
    cin >> n >> m >> k;
    Dinic d = Dinic(n+m+2);
    for (int i = 1; i <= n; i++) {
        d.addEdge(0,i,1);
    }
    for (int i = n+1; i <= n+m; i++) {
        d.addEdge(i,n+m+1,1);
    }
    for (int i = 1; i <= k; i++) {
        int a, b;
        cin >> a >> b;
        d.addEdge(a, b+n, 1);
    }
    cout<<d.calc(0, n+m+1)<<'\n';
    for (int i = 1; i <=n; i++) {
        for (auto e : d.adj[i]) {
            if (e.flow()>0) {
                cout << i << ' ' << e.to-n << '\n';
            }
        }
    }
}
void Distinct_Routes() {
    int n,m;
    cin>>n>>m;
    Dinic d(n+1);
    vvi g(n+1);
    for(int i=1;i<=m;i++)
    {
        int a,b;
        cin>>a>>b;
        d.addEdge(a,b,1);
        g[a].pb(b);
    }
    int k = d.calc(1,n);
    cout<<k<<endl;
    auto &adj = d.adj;
    vector<vii> u(n+1);
    for(int i=1;i<=n;i++)
    {
        for (int j = 0; j < sz(adj[i]); j++){
            if (adj[i][j].flow()==1)
            {
                u[i].pb(make_pair(adj[i][j].to,j));
            }
        }
    }
    vi path;
    path.pb(1);
    rep(i,0,k)
    {
        path = vi();
        path.pb(1);
        buildpath(1,u,n,path);
    }
}
void Police_Chase() {
    int n,m;
    cin>>n>>m;
    vii g;
    Dinic d(n);
    for(int i=0;i<m;i++) {
        int a,b;
        cin>>a>>b;
        a--;
        b--;
        g.pb(make_pair(a,b));
        d.addEdge(a,b,1);
        d.addEdge(b,a,1);
    }
    int k = d.calc(0,n-1);
    cout<<k<<endl;
    for (auto e : g) {
        if (d.leftOfMinCut(e.first)!=d.leftOfMinCut(e.second)) {
            cout<<e.second+1<<' '<<e.first+1<<' '<<endl;
        }
    }
}
void Task_Assignment() {
    int n;
    cin >> n;
    vector<vi> a(n);
    for (int i = 0; i < n; i++) {
        a[i].resize(n);
    }
    rep (i, 0, n) {
        rep (j, 0, n) cin >> a[i][j];
    }
    auto ans = hungarian(a);
    cout<<ans.first<<'\n';
    for (int i = 0; i<a.size(); i++) {
        cout<<i+1<<" "<<ans.second[i]+1<<'\n';
    }
}
void Distinct_Routes_II() {
    int n,m,k;
    cin>>n>>m>>k;
    vector<tuple<int,int,int>> g;
    MCMF d(n+1);
    for(int i=0;i<m;i++) {
        int a,b;
        cin>>a>>b;
        a--;
        b--;
        g.pb({a,b, d.ed[a].size()});
        d.addEdge(a,b,1,1);
    }
    d.addEdge(n-1,n,k,0);
    auto f = d.maxflow(0,n);
    if (f.first!=k) {
        cout<<-1;
        return;
    }
    cout<<f.second<<'\n';
    vvii used(n);
    rep(i,0,m) {
        auto [u,v,index] = g[i];
        if (d.ed[u][index].flow != 0) {
            used[u].push_back({v,index});
        }
    }
    vi used2(m,0);
    rep(i,0,k) {
        vi path;
        path.pb(0);
        buildpath2(0, used, n - 1, path);
    }

}
void Parcel_Delivery() {
    int n,m,k;
    cin>>n>>m>>k;
    MCMF d(n+1);
    for(int i=0;i<m;i++) {
        int a,b,w,c;
        cin>>a>>b>>w>>c;
        a--;
        b--;
        d.addEdge(a,b,w,c);
    }
    d.addEdge(n-1,n,k,0);
    auto f = d.maxflow(0,n);
    if (f.first!=k) {
        cout<<-1;
        return;
    }
    cout<<f.second<<'\n';

}
void Coin_Collector() {
    int n,m;
    cin>>n>>m;
    vi coin(n);
    for(int i=0;i<n;i++) {
        cin>>coin[i];
    }
    vvi adj(n);
    vii e;
    rep(i,0,m) {
        int a,b;
        cin>>a>>b;
        a--;
        b--;
        adj[a].pb(b);
        e.push_back({a,b});
    }
    vi comp = SCCKosaraju::scc(adj);
    int c = 0;
    rep(i,0,n) {
        c = max(c, comp[i]+1);
    }
    vi cCoin(c,0);
    rep(i,0,n) {
        cCoin[comp[i]] += coin[i];
    }

    queue <int> q;
    vi in(c,0);
    vvi g(c);
    for (auto i : e) {
        if (comp[i.first]!=comp[i.second]) {
            g[comp[i.first]].pb(comp[i.second]);
            in[comp[i.second]]++;
        }
    }
    vi dp = cCoin;
    for (int i = 0; i<c; i++) {
        if (in[i]==0) {
            q.push(i);
        }
    }
    int ans = 0;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        ans = max(ans, dp[u]);
        for (int v:g[u]) {
            dp[v] = max(dp[v], dp[u]+cCoin[v]);
            in[v]--;
            if (in[v]==0) {
                q.push(v);
            }
        }
    }
    cout << ans << '\n';
}
void Delivery_Bears() {
    int n,m,x;
    cin>>n>>m>>x;
    vector<tuple<int,int,int>> edges;
    double high = 0;
    rep(i,0,m) {
        int a,b,c;
        cin>>a>>b>>c;
        a--;
        b--;
        edges.pb({a,b,c});
        high = max(high,(double)c);
    }
    double low = 0;
    rep(it,0,100) {
        double mid = (low+high)/2;
        Dinic d(n);
        for(auto [a,b,c]:edges) {
            ll bears = min((ll)(c/mid),(ll)x);
            d.addEdge(a,b,bears);
        }
        if(d.calc(0,n-1)>=x) {
            low = mid;
        }
        else {
            high = mid;
        }
    }
    cout<<setprecision(30)<<low*x<<'\n';
}


void Ethan_and_the_Hungry_Debuggers() {
    int n,m;
    cin>>n>>m;
    vector<vector<char>> a(n+2,vector<char>(m+2,'.'));
    for(int i=0;i<n;i++) {
        for(int j=0;j<m;j++) {
            char c;
            cin>>c;
            a[i+1][j+1]=c;
        }
    }
    int count = 0;
    for(int i=0;i<n;i++) {
        for(int j=0;j<m;j++) {
            char c;
            c = a[i+1][j+1];
            if (c=='W') {
                if (a[i+1][j]=='P' || a[i][j+1]=='P' || a[i+2][j+1]=='P' || a[i+1][j+2]=='P')
                    count++;
            }
        }
    }
    cout<<count;

}
void Ethan_kidnaps_students() {
    int n,m;
    cin>>n>>m;
    vi a(n),b(n);
    int sum = 0;
    rep(i,0,n) cin>>a[i];
    rep(i,0,n){ cin>>b[i]; sum += b[i]; }
    Dinic d = Dinic(2*n+2);
    for (int i = 1; i <= n; i++) {
        d.addEdge(0, i, a[i-1]);
    }
    for (int i = 1; i <= n; i++) {
        d.addEdge(i+n, 2*n+1, b[i-1]);
    }
    for (int i = 0; i<m ; i++) {
        int u, v;
        cin>>u>>v;
        d.addEdge(u, v+n, sum);
        d.addEdge(v, u+n, sum);
    }
    for (int i = 1; i <= n; i++) {
        d.addEdge(i, i+n, sum);
    }
    if (m==0) {
        for (int i = 0; i < n; i++) {
            if (b[i]!=a[i]) {
                cout<<"NO"<<"\n";
                return;
            }
        }
    }
    ll r = d.calc(0, 2*n+1);
    if (r<sum) {
        cout<<"NO";
        return;
    }
    cout<<"YES"<<"\n";
    vector<vector<ll>> f(n,vector<ll>(n,0));
    for(int i=1;i<=n;i++) {
        for (auto j : d.adj[i]) {
            if (n + 1 <= j.to && j.to <= 2 * n) {
                f[i - 1][j.to - n - 1] += j.flow();
            }
        }
    }
    for(int i=0;i<n;i++) {
        for (int j=0;j<n;j++) {
            cout << f[i][j] << " ";
        }
        cout << "\n";
    }

}
void Shalev_and_the_Downward_Tree() {
    int n;
    cin>>n;
    vi x(n),y(n);
    rep(i,0,n)
        cin>>x[i]>>y[i];
    int source = 2*n;
    int sink = 2*n+1;
    double_MCMF d(2*n+3);
    rep(i,0,n) {
        d.addEdge(source,i,1,0);
        d.addEdge(n+i,sink,2,0);
    }
    rep(child,0,n) {
        rep(parent,0,n) {
            if(y[parent]>y[child]) {
                double dx=x[parent]-x[child];
                double dy=y[parent]-y[child];
                double cost=sqrt(dx*dx+dy*dy);
                d.addEdge(child,n+parent,1,cost);
            }
        }
    }
    auto f=d.maxflow(source,sink);
    if(f.first!=n-1) {
        cout<<-1<<'\n';
        return;
    }
    cout<<setprecision(30)<<f.second<<'\n';
}
void The_Resistance() {
    int n,m;
    cin>>n>>m;
    const int inf=1e9;
    vvi dist(n,vi(n,inf));
    rep(i,0,n)
        dist[i][i]=0;
    rep(i,0,m) {
        int u,v;
        cin>>u>>v;
        u--;
        v--;
        dist[u][v]=1;
        dist[v][u]=1;
    }

    rep(k,0,n) {
        rep(i,0,n) {
            rep(j,0,n) {
                dist[i][j]=min(dist[i][j],dist[i][k]+dist[k][j]);
            }
        }
    }
    int s,b;
    int k,h;
    cin>>s>>b>>k>>h;
    vi shipLocation(s);
    vector<ll> attack(s),fuel(s);
    rep(i,0,s) {
        cin>>shipLocation[i]>>attack[i]>>fuel[i];
        shipLocation[i]--;
    }
    vi baseLocation(b);
    vector<ll> defense(b);
    rep(i,0,b) {
        cin>>baseLocation[i]>>defense[i];
        baseLocation[i]--;
    }
    int source=0;
    int sink=s+b+1;
    Dinic d(s+b+2);
    rep(i,0,s) {
        d.addEdge(source,1+i,1);
    }
    rep(j,0,b) {
        d.addEdge(1+s+j,sink,1);
    }
    rep(i,0,s) {
        rep(j,0,b) {
            if(attack[i]>=defense[j] && fuel[i]>=dist[shipLocation[i]][baseLocation[j]])
                d.addEdge(1+i,1+s+j,1);
        }
    }
    ll matching=d.calc(source,sink);
    cout<<min(matching*k,s*h)<<'\n';
}
void Ethan_obsessed_with_beauty_standards() {
    int n,m;
    cin>>n>>m;
    vvi adj(n);
    rep(i,0,m) {
        int u,v;
        cin>>u>>v;
        u--;
        v--;
        adj[u].pb(v);
        adj[v].pb(u);
    }
    vi color(n,-1);
    ll ans=1;
    rep(start,0,n) {
        if(color[start]!=-1)
            continue;
        queue<int> q;
        q.push(start);
        color[start]=0;
        int cnt[2]={1,0};
        bool possible=true;
        while(!q.empty()) {
            int u=q.front();
            q.pop();
            for(int v:adj[u]) {
                if(color[v]==-1) {
                    color[v]=color[u]^1;
                    cnt[color[v]]++;
                    q.push(v);
                }
                else if(color[v]==color[u]) {
                    possible=false;
                }
            }
        }
        if(!possible) {
            cout<<0<<'\n';
            return;
        }
        ans=ans*(power2[cnt[0]]+power2[cnt[1]])%MOD;
    }
    cout<<ans<<'\n';
}
void Shalev_and_Optimus_Prime() {
    int n;
    cin>>n;
    vi a(n);
    rep(i,0,n)
        cin>>a[i];
    vi prime(20001,1);
    prime[0]=prime[1]=0;
    for(int i=2;i*i<=20000;i++) {
        if(prime[i]) {
            for(int j=i*i;j<=20000;j+=i) {
                prime[j]=0;
            }
        }
    }
    int source=n;
    int sink=n+1;
    Dinic d(n+2);
    rep(i,0,n) {
        if(a[i]%2) {
            d.addEdge(source,i,2);
        }
        else {
            d.addEdge(i,sink,2);
        }
    }
    rep(i,0,n) {
        if(a[i]%2==0)
            continue;
        rep(j,0,n) {
            if(a[j]%2==0 && prime[a[i]+a[j]]) {
                d.addEdge(i,j,1);
            }
        }
    }
    if(d.calc(source,sink)!=n) {
        cout<<"Impossible\n";
        return;
    }
    vvi g(n);
    rep(i,0,n) {
        if(a[i]%2==0)
            continue;
        for(auto e:d.adj[i]) {
            if(e.to<n && e.flow()==1) {
                g[i].pb(e.to);
                g[e.to].pb(i);
            }
        }
    }
    vi visited(n,0);
    vvi tables;
    rep(start,0,n) {
        if(visited[start])
            continue;
        vi table;
        int previous=-1;
        int current=start;
        do {
            table.pb(current);
            visited[current]=1;
            int next;
            if(g[current][0]!=previous) {
                next=g[current][0];
            }
            else {
                next=g[current][1];
            }
            previous=current;
            current=next;
        }
        while(current!=start);
        tables.pb(table);
    }
    cout<<tables.size()<<'\n';
    for(auto table:tables) {
        cout<<table.size()<<" ";

        for(int robot:table) {
            cout<<robot+1<<" ";
        }
        cout<<'\n';
    }
}
void Shalev_and_the_Reactor_Panic() {
    int n,t;
    cin>>n>>t;
    vector<string> people(n),capsules(n);
    rep(i,0,n) cin>>people[i];
    rep(i,0,n) cin>>capsules[i];

    const int inf=1e9;
    vvi infection(n,vi(n,inf));
    queue<pii> q;
    rep(i,0,n) {
        rep(j,0,n) {
            if(people[i][j]=='Z') {
                infection[i][j]=0;
                q.push({i,j});
            }
        }
    }
    int dx[4]={1,-1,0,0};
    int dy[4]={0,0,1,-1};
    while(!q.empty()) {
        auto [x,y]=q.front();
        q.pop();

        rep(d,0,4) {
            int nx=x+dx[d];
            int ny=y+dy[d];

            if(nx<0 || nx>=n || ny<0 || ny>=n)
                continue;

            if(people[nx][ny]<'0' || people[nx][ny]>'9')
                continue;

            if(infection[nx][ny]==inf) {
                infection[nx][ny]=infection[x][y]+1;
                q.push({nx,ny});
            }
        }
    }

    int source=2*n*n;
    int sink=source+1;
    Dinic flow(sink+1);
    rep(i,0,n) {
        rep(j,0,n) {
            if(people[i][j]=='Y' || people[i][j]=='Z')
                continue;
            int start=i*n+j;
            int shelter=n*n+i*n+j;
            flow.addEdge(source,start,people[i][j]-'0');
            flow.addEdge(shelter,sink,capsules[i][j]-'0');
            vvi dist(n,vi(n,-1));
            queue<pii> bfs;
            dist[i][j]=0;
            bfs.push({i,j});
            while(!bfs.empty()) {
                auto [x,y]=bfs.front();
                bfs.pop();
                int shelterCell=n*n+x*n+y;
                flow.addEdge(start,shelterCell,inf);
                if(dist[x][y]>=infection[x][y])
                    continue;
                rep(d,0,4) {
                    int nx=x+dx[d];
                    int ny=y+dy[d];
                    if(nx<0 || nx>=n || ny<0 || ny>=n)
                        continue;
                    if(people[nx][ny]<'0' || people[nx][ny]>'9')
                        continue;
                    int arrival=dist[x][y]+1;
                    if(arrival>t)
                        continue;
                    if(arrival>infection[nx][ny])
                        continue;
                    if(dist[nx][ny]!=-1)
                        continue;
                    dist[nx][ny]=arrival;
                    bfs.push({nx,ny});
                }
            }
        }
    }
    cout<<flow.calc(source,sink)<<'\n';
}




signed main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int t = 1;
    //cin>>t;
    /*power2[0]=1;
    rep(i,1,MAXN) {
        power2[i]=power2[i-1]*2%MOD;
    }*/
    //for Ethan_obsessed_with_beauty_standards
    while (t--) {
        //the problem:

    }
    return 0;
}
