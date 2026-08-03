#include <bits/stdc++.h>
#define int long long
#define rep(i,x,n) for(int i=x;i<n;i++)
using namespace std;
#define INF LONG_LONG_MAX
#define vi vector<int>
#define pb push_back
#define vii vector<pair<int,int>>
#define vvi vector<vi>
#define vvii vector<vvi>
#define eb emplace_back
#define all(v) v.begin(),v.end()
#define pii pair<int,int>

// "macros"
vi si, p, a;
void pre(int size) {
    p.resize(size);
    si.resize(size);
    for (int i = 0; i < size; i++) {
        p[i] = i;
        si[i] = 1;
    }
}
int find(int x) {
    if (x == p[x])
        return x;
    return p[x] = find(p[x]);
}
int onion(int a, int b) {
    if ((a=find(a)) == (b=find(b))) {
        return 0;
    }
    if (si[a] < si[b]) {
        swap(a, b);
    }
    return si[a] += si[b], p[b] = a, 1;
}
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
}
void dijkstra(int n, vector<vector<pii>> &g, int s, vector<int> &dist, vector<int> &pr) {
    rep(i,0,n) {
        dist[i] = LONG_LONG_MAX/4;
        pr[i] = -1;
    }
    priority_queue<pii, vector<pii>, greater<pii>> pq;
    pq.push(pii(0, s));
    dist[s] = 0;
    while(!pq.empty()) {
        pii p = pq.top(); pq.pop();
        int d = p.first;
        int u = p.second;
        if (dist[u] < d) { continue; }
        for (auto v : g[u]) {
            int next = v.first;
            int w = v.second;
            if (dist[next] > dist[u] + w) {
                dist[next] = dist[u] + w;
                pq.push(pii(dist[next], next));
                pr[next] = u;
            }
        }
    }
}
vector<int> dijkstra_cahnged(vector<vector<pair<int,int>>>& adj, vi &cost) {
    int V = adj.size();
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    vector<int> dist(V, INT_MAX);
    for (int i = 0; i < cost.size(); i++) {
        dist[i] = cost[i];
        pq.push({cost[i], i});
    }
    while (!pq.empty()) {
        auto top = pq.top();
        pq.pop();
        int d = top.first;
        int u = top.second;
        // If this distance not the latest shortest one, skip it
        if (d > dist[u])
            continue;
        for (auto &p : adj[u]) {
            int v = p.first;
            int w = p.second;
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }
    return dist;
}
vi bfs(int s, vvi& g) {
    queue<pii> q;
    vi dist(g.size(), -1);
    vi vis(g.size());
    q.push(pii(s, 0));
    vis[s]=1;
    while (q.size()) {
        int u = q.front().first;
        int d = q.front().second;
        q.pop();
        dist[u] = d;
        for (auto v : g[u]) {
            if (vis[v]) continue;
            q.push(pii(v, d+1));
            vis[v] = 1;
        }
    }
    return dist;
}

//HW
void Secret_Passwords() {
    int n;
    cin >> n;
    pre(n+26);
    vector<string> p(n);
    for (int i = 0; i < n; i++) {
        cin >> p[i];
    }
    for (int i = 0; i < n; i++) {
        for (auto c : p[i]) {
            onion(i,c-'a'+n);
        }
    }
    set<int> s;
    for (int i = 0; i < n; i++) {
        s.insert(find(i));
    }
    cout << s.size();
}
void Flight_Routes_Check() {
    int n,m;
    cin >> n >> m;
    vvi g(n);
    for (int i = 0; i < m; i++) {
        int u,v;
        cin >> u >> v;
        u--;
        v--;
        g[u].pb(v);
    }
    vi scc_index = SCCKosaraju::scc(g);
    for (int i = 1; i < n; i++) {
        if (scc_index[i] != scc_index[0]) {
            if (scc_index[i]>scc_index[0]) {
                cout << "NO\n";
                cout << i+1 << " " << 1 << endl;
                return;
            }
            cout << "NO\n";
            cout << 1 << " " << i+1 << endl;
            return ;
        }
    }
    cout << "YES";
}
void Planets_And_Kingdoms() {
    int n,m;
    cin>>n>>m;
    vector<vector<int>> adj(n);
    rep(i,0,m)
    {
        int a,b;
        cin>>a>>b;
        a--;
        b--;
        adj[a].push_back(b);

    }
    vector<int> comp = SCCKosaraju::scc(adj);
    set<int> s;
    for (int x: comp)
        s.insert(x);
    cout<<s.size()<<'\n';
    for(int x : comp) cout<<x+1<<" ";
}
void Message_Route() {
    int n,m;
    int x,y;
    vector<int> dis(0);
    queue<int> q;
    cin>>n>>m;
    vector<int> vis(n, 0);
    vector<int> parent(n, 0);
    dis.assign(n,0);
    vector<int> p[n];
    for(int i=0;i<m;i++)
    {
        cin>>x>>y;
        p[x-1].push_back(y-1);
        p[y - 1].push_back(x - 1);
    }
    vis[0]=1;
    dis[0]=0;
    q.push(0);
    parent[0]=-1;
    while(!q.empty())
    {
        auto front = q.front();
        q.pop();
        for (auto v:p[front])
        {
            if(!vis[v])
            {
                vis[v]=1;
                q.push(v);
                parent[v]=front;
                dis[v]=dis[front]+1;
            }
        }
    }
    if(dis[n-1]==0)
        cout<<"IMPOSSIBLE";
    else
    {
        vector<int> path_vector;
        for (int curr = n - 1; curr != -1; curr = parent[curr]) {
            path_vector.push_back(curr + 1);
        }
        cout<<dis[n-1]+1<<"\n";
        for (int i = path_vector.size()-1; i >= 0; i--)
        {
            cout<<path_vector[i]<<" ";
        }
    }
}
void Dijkstra_qm(){
    int n,m;
    cin >> n >> m;
    vector<vector<pii>> g(n);
    for (int i = 0; i < m; i++) {
        int u,v,w;
        cin >> u >> v >> w;
        u--; v--;
        g[u].pb({v,w});
        g[v].pb({u,w});
    }
    vi dist(n);
    vi pr(n);
    dijkstra(n, g, 0,  dist, pr);
    if (dist[n-1]==LONG_LONG_MAX/4) {
        cout << -1 << endl;
    }
    else {
        vi path;
        for (int i = n-1; i!=-1; i=pr[i]) {
            path.pb(i);
        }
        reverse(path.begin(), path.end());
        for (int node : path) {
            cout << node + 1<< " ";
        }
        cout << endl;
    }

}
void Strongly_Connected_City() {
    int n,m;
    cin >> n >> m;
    string s1; cin >> s1;
    string s2; cin >> s2;
    string s3 = { s1[0] , s1[n-1] , s2[0] , s2[m-1] };
    cout << (s3 == "<>v^" || s3 == "><^v" ? "YES" : "NO");
}
void Building_Roads() {
    int n,m;
    cin >> n >> m;
    pre(n);
    for (int i = 0; i < m; i++) {
        int u,v;
        cin >> u >> v;
        u--; v--;
        onion(u,v);
    }
    set<int> s;
    for (int i = 0; i < n; i++) {
        s.insert(find(i));
    }
    cout << s.size()-1 << '\n';
    for (auto c : s) {
        if (c!=find(0)) {
            cout << c + 1 <<" " << 1 << '\n';
        }
    }

}
void Learning_Languages() {
    int n,m;
    cin >> n >> m;
    pre(n+m);
    int flag = 0;
    for (int i = 0; i < n; i++) {
        int k;
        cin >> k;
        if (k) flag = 1;
        for (int j = 0; j < k; j++) {
            int lan = -1;
            cin >> lan;
            lan+=n-1;
            onion(i,lan);
        }
    }
    set<int> s;
    for (int i = 0; i < n; i++) {
        s.insert(find(i));
    }
    if (!flag) {
        cout<<n;
        return;
    }
    cout << s.size()-1 << '\n';

}
void Fox_And_Names() {
    int n;
    cin >> n;
    vvi g(26);
    queue<int> q;
    vector<int> in(26, 0);
    string s; cin >> s;
    for (int i = 1; i < n; i++) {
        string u; cin >> u;
        int j = 0;
        while (j < s.length() && j < u.length() && u[j] == s[j]) {
            j++;
        }
        if (j == u.length() && j < s.length()) {
            cout << "Impossible";
            return;
        }
        if (j == s.length() || j == u.length()) {
            s = u;
        }
        else {
            g[s[j]-'a'].pb(u[j]-'a');
            in[u[j]-'a']++;
            s = u;
        }
    }
    for(int i = 0; i < 26; i++) if(in[i] == 0){
        q.push(i);
    }
    vector<int> topo;
    int cnt = 0;
    while(!q.empty()){
        int v = q.front();
        q.pop();
        cnt++;
        topo.pb(v);
        for(auto u : g[v]){
            in[u]--;
            if(in[u] == 0) q.push(u);
        }
    }
    if (topo.size() < 26) {
        cout << "Impossible";
    }
    else {
        for (int v : topo) {
            cout << char(v + 'a');
        }
    }
}
void two_buttons() {
    int n,m;
    cin >> n >> m;
    vvi g(2LL*max(n,m)+1LL);
    for (int i = 0; i <= 2LL*max(n,m); i++) {
        if (i*2LL<2LL*max(n,m)) {
            g[i].push_back(i*2LL);
        }
        if (i!=0) {
            g[i].push_back(i-1);
        }
    }
    vi dist = bfs(n,g);
    cout << dist[m];
}
void News_Distribution() {
    int n,m;
    cin >> n >> m;
    pre(n);
    for (int i = 0; i < m; i++) {
        int k;
        int v;
        cin >> k;
        if (k) {
            k--;
            cin >> v;
            v--;
        }
        for (int j = 0; j < k; j++) {
            int u;
            cin >> u;
            u--;
            onion(v,u);
        }
    }
    for (int i = 0; i < n; i++) {
        cout << si[find(i)] << " ";
    }
}
void Checkposts() {
    int n,m;
    cin >> n;
    vector<vector<int>> g(n);
    vi cost(n);
    for (int i = 0; i < n; i++) {
        cin >> cost[i];
    }
    cin>>m;
    for (int i = 0; i < m; i++) {
        int u,v;
        cin >> u >> v;
        u--; v--;
        g[u].pb(v);
    }
    vi scc_index = SCCKosaraju::scc(g);
    set<int> s;
    for (int i = 0; i < n; i++) {
        s.insert(scc_index[i]);
    }
    vi mincost(s.size(), LLONG_MAX);
    for (int i = 0; i < n; i++) {
        if (mincost[scc_index[i]] > cost[i]) {
            mincost[scc_index[i]] = cost[i];
        }
    }
    vi scc_cnt(s.size());
    int co = 0;
    for (int i = 0; i < s.size(); i++) {
        co+=mincost[i];
    }
    for (int i = 0; i < n; i++) {
        if (cost[i] == mincost[scc_index[i]])
            scc_cnt[scc_index[i]]++;
    }
    cout << co << " ";
    int cnt = scc_cnt[0];
    for (int i = 1; i < s.size(); i++) {
        cnt*=scc_cnt[i];
        cnt%=1000000007LL;
    }
    cout << cnt;
}
void Buy_a_Ticket() {
    int n,m;
    cin >> n >> m;
    vector<vector<pii>> adj(n);
    for (int i = 0; i < m; i++) {
        int u,v,w;
        cin >> u >> v >> w;
        u--; v--;
        adj[u].pb({v,2*w});
        adj[v].pb({u,2*w});
    }
    vi cost(n);
    for (int i = 0; i < n; i++) {
        cin >> cost[i];
    }
    vi fast = dijkstra_cahnged(adj,cost);
    for (int i = 0; i < n; i++) {
        cout << fast[i] << " ";
    }
}

signed main() {
    cin.tie(0)->sync_with_stdio(false), cout.tie(0);
    return 0;
}
