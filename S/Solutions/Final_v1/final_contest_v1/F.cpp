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

void Euler_Tour(vvi& g, int& time, vector<pii>& times, vi& nodes, int source) {
    nodes[time] = source;
    times[source].first = time++;
    for (auto u : g[source]) {
        Euler_Tour(g, time, times, nodes, u);
    }
    times[source].second = time;
}
int bfs(vvi& g, int source, vi& d) {
    queue<int> q; q.push(source); d[source] = 0;
    int max_depth = 0;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (auto v : g[u]) {
            d[v] = d[u]+1;
            max_depth = max(max_depth, d[v]);
            q.push(v);
        }
    }
    return max_depth;
}
void Shalev_Accidentally_Started_a_Cult() {
    int n; cin >> n;
    int m; cin >> m;

    vvi g(n);
    rep(i,1,n) {
        int p; cin >> p; p--;
        g[p].push_back(i);
    }

    string letters; cin >> letters;

    int time = 0;
    vector<pii> times(n); // maps a node to its time
    vi nodes(n); // maps time to its node
    Euler_Tour(g, time, times, nodes, 0);

    vi d(n);
    int max_depth = bfs(g, 0, d);

    vvi depths(max_depth+1,vi(1,-1)); // maps for each depth the times of the nodes of this depth
    vector<vector<bitset<26>>> depths_masks(max_depth+1,vector<bitset<26>>(1,0)); // maps for each depth a bitset xor_prefix vector
    rep(i,0,n) {
        int u = nodes[i];
        bitset<26> temp = depths_masks[d[u]].back();
        temp[letters[u] - 'a'] = temp[letters[u] - 'a']^1;
        depths_masks[d[u]].push_back(temp);

        depths[d[u]].push_back(i);
    }

    rep(i,0,m) {
        int v,h; cin >> v >> h; v--; h--;
        if (h>max_depth) { cout << "Yes\n"; continue; }

        // the range of the current request
        int l = lower_bound(depths[h].begin(),depths[h].end(),times[v].first) - depths[h].begin(); l--;
        int r = lower_bound(depths[h].begin(),depths[h].end(),times[v].second) - depths[h].begin(); r--;

        bitset<26> temp = depths_masks[h][r]^depths_masks[h][l];

        if (temp.count() > 1) cout << "No\n";
        else cout << "Yes\n";
    }
}


signed F() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int t = 1;
    //cin >> t;
    while (t--) {
        Shalev_Accidentally_Started_a_Cult();
        cout<<"\n";
    }
    return 0;
}
