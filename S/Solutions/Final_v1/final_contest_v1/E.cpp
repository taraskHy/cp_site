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

void save(int root, vvi& adj, vi&seen, vi& dp, vi& size) {
    seen[root] = 1;
    size[root] = 1;
    vi children;
    for (auto i: adj[root]) {
        if (!seen[i]) {
            children.push_back(i);
        }
    }
    if (children.size() == 0) {
        dp[root] = 0;
        return;
    }
    if (children.size() == 1) {
        save(children[0], adj, seen, dp, size);
        dp[root] = size[children[0]]-1;
        size[root]+= size[children[0]];
    }
    else {
        save(children[0], adj, seen, dp, size);
        save(children[1], adj, seen, dp, size);
        dp[root] = max(size[children[0]]+dp[children[1]], dp[children[0]]+size[children[1]])-1;
        size[root]+= size[children[0]];
        size[root]+= size[children[1]];
    }
    return;
}
void Ethan_Downloaded_Free_RAM() {
    int n;
    cin>>n;
    vvi adj(n,vi());
    for (int i = 0; i < n-1; i++) {
        int u, v;
        cin>>u>>v;
        u--;
        v--;
        adj[u].pb(v);
        adj[v].pb(u);
    }
    vi dp(n,-1);
    vi seen(n,0);
    vi size(n,0);
    save(0, adj, seen, dp, size);
    cout << dp[0];
}

signed E() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int t = 1;
    cin >> t;
    while (t--) {
        Ethan_Downloaded_Free_RAM();
        cout<<"\n";
    }
    return 0;
}
