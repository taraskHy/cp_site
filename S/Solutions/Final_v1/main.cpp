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

struct Node {
    int count = 0;
    Node* left = NULL;
    Node* right = NULL;
};
void Shalev_and_the_Infinity_Multiset() {
    int q;
    cin>>q;
    multiset<unsigned int> ms;
    ms.insert(0);
    Node *root = new Node();
    bitset<32> xs(0U);
    root->count++;
    Node* temp = root;
    int i = 31;
    while (i!=-2 && temp->count != 0) {
        if (temp!=root) temp->count++;
        if (i==-1) {
            temp->count++;
            i--;
            continue;
        }
        if (!xs[i]) {
            if (!temp->left)
                break;
            temp = temp->left;
        }
        else {
            if (!temp->right)
                break;
            temp = temp->right;
        }
        i--;
    }
    while (i!=-1) {
        Node *curr = new Node();
        curr->count = 1;
        if (!xs[i]) {
            temp->left = curr;
        }
        else {
            temp->right = curr;
        }
        temp = curr;
        i--;
    }

    while(q--) {
        char c;
        cin>>c;
        switch(c) {
            case '+': {
                unsigned int x;
                cin>>x;
                bitset<32> xs(x);
                if (ms.find(x) == ms.end()) {
                    root->count++;
                    Node* temp = root;
                    int i = 31;
                    while (i!=-2 && temp->count != 0) {
                        if (temp!=root) temp->count++;
                        if (i==-1) {
                            temp->count++;
                            i--;
                            continue;
                        }
                        if (!xs[i]) {
                            if (!temp->left)
                                break;
                            temp = temp->left;
                        }
                        else {
                            if (!temp->right)
                                break;
                            temp = temp->right;
                        }
                        i--;
                    }
                    while (i!=-1) {
                        Node *curr = new Node();
                        curr->count = 1;
                        if (!xs[i]) {
                            temp->left = curr;
                        }
                        else {
                            temp->right = curr;
                        }
                        temp = curr;
                        i--;
                    }
                }
                else {
                    Node *curr = root;
                    int i = 31;
                    bitset<32> xs(x);
                    Node *prev;
                    while (curr!=NULL && i!=-1) {
                        curr->count++;
                        if (!xs[i]) {
                            curr = curr->left;
                        }
                        else {
                            curr = curr->right;
                        }
                        i--;
                    }
                    curr->count++;
                }
                ms.insert(x);
                break;
            }
            case '-': {
                unsigned int x;
                cin>>x;
                ms.erase(ms.find(x));
                Node *curr = root;
                bitset<32> xs(x);
                curr->count--;
                for (int i = 31; i >= 0; i--) {
                    Node *next;
                    if (!xs[i])
                        next = curr->left;
                    else
                        next = curr->right;
                    next->count--;
                    if (next->count == 0) {
                        if (!xs[i])
                            curr->left = NULL;
                        else
                            curr->right = NULL;
                        break;
                    }
                    curr = next;
                }
                break;
            }

            case '?': {
                unsigned int x;
                cin>>x;
                bitset<32> xs(x);
                bitset<32> n;
                Node *curr = root;
                for (int i = 31; i >= 0 ; i--) {
                    if (xs[i]&&curr->left) {
                        curr = curr->left;
                        n[i] = 1;
                    }
                    else if (!xs[i]&&curr->right) {
                        curr = curr->right;
                        n[i] = 1;
                    }
                    else {
                        curr = (curr->left ? curr->left : curr->right);
                        n[i] = 0;
                    }
                }
                cout << n.to_ulong() << endl;
            }
            default: ;
        }
    }
}

int prexor(int x) {
    if (x==-1) return 0;
    if (x%4==0) {
        return x;
    }
    if (x%4==3) {
        return 0;
    }
    if (x%4==1) {
        return 1;
    }
    return x+1;
}
void Shalevs_Minecraft_Industrial_Revolution() {
    int n;
    cin>>n;
    int ans = 0;
    for (int i = 0; i < n; i++) {
        int x,m;
        cin>>x;
        cin>>m;
        int a = prexor(x-1);
        int b = prexor(x+m-1);
        ans ^= a^b;
    }
    cout << (ans? "tolik" : "bolik") << endl;

}

void Shalev_Forgot_to_Shut_Down_the_Server() {
    int n; cin >> n;
    //int m; cin >> m;

    vi a(n); rep(i,0,n) cin >> a[i];

    vi substring_length_sum(32,0); //sum of substring length where bit i is on
    vi suffix_count(32,0); //number of suffixes where bit i is on
    vi suffix_length_sum(32,0); //sum of suffix length where bit i is on

    rep(i,0,n) {
        int current = a[i];
        rep(j,0,32) {
            int bit = current%2;
            current/=2;

            if (bit == 0) {
                //since bit j in a[i] is 0, all suffixes that had bit j on earlier, have it on now

                //each suffix with bit j on from earlier is now 1 element longer
                suffix_length_sum[j] += suffix_count[j];
                suffix_length_sum[j] %= MOD;
                substring_length_sum[j] += suffix_length_sum[j];
                substring_length_sum[j] %= MOD;
            }

            if (bit == 1) {
                //since bit j in a[i] is 1, all suffixes that had bit j off earlier, have it on now
                suffix_count[j] = i - suffix_count[j];
                suffix_length_sum[j] = (((i+1)*i)/2 - suffix_length_sum[j])%MOD;

                //the last element, a[i], becomes itself a suffix with bit j on
                suffix_count[j]++;

                //each suffix with bit j off from earlier is now 1 element longer
                suffix_length_sum[j] += suffix_count[j];
                suffix_length_sum[j] %= MOD;
                substring_length_sum[j] += suffix_length_sum[j];
                substring_length_sum[j] %= MOD;
            }
        }
    }

    int current_power = 1;
    int ans = 0;
    rep(i,0,32) {
        ans += substring_length_sum[i] * current_power;
        ans %= MOD;
        current_power *= 2;
        current_power %= MOD;
    }

    cout << ans << endl;
}

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

void Ethan_and_the_XORting_Hat(){
    int n;
    cin>>n;
    vi a(n);
    map<int, priority_queue<int>> mp;
    for(int i=0; i<n; i++){
        cin>>a[i];
        mp[a[i]>>2].push(-a[i]);
    }
    for(int i=0; i<n; i++){
        cout<<-mp[a[i]>>2].top() << " ";
        mp[a[i]>>2].pop();
    }
}

void Ethan_and_the_Group_Project_From_Hell() {
    int n;
    cin>>n;
    vi ans(n);
    vi b((n*(n-1))/2);
    multiset<int> ms;
    for (int i = 0; i < n*(n-1)/2; i++) {
        cin>>b[i];
    }
    sort(all(b));
    int index = 0;
    for (int i = 0; i< n-1; i++) {
        ans[i] = b[index];
        index+=n-i-1;
    }
    ans[n-1] = ans[n-2];
    for (auto i: ans) {
        cout << i << " ";
    }

}



signed main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int t = 1;
    //cin >> t;
    while (t--) {
        cout<<"\n";
    }
    return 0;
}
