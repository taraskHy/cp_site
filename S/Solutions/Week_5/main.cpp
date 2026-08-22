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
vi euler;
vi tin,tout;
vi visited;
int t = -1;
string st;
struct segment_tree_arr_euler{
    int n;
    vi value;
    segment_tree_arr_euler(int n): value(4*n, 0), n(n){
        build(1, 1, n);
    }

    void build(int node, int l, int r){
        if(l == r) {
            value[node] = euler[l-1];
            return;
        }
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
void euler_tour_val(vvi& adj, int s) {
    visited[s] = true;
    t++;
    tin[s] = t;
    euler[t] = a[s];
    for (auto i : adj[s]) {
        if (!visited[i]) {
            euler_tour_val(adj, i);
        }
    }
    t++;
    tout[s] = t;
    euler[t] = -a[s];
}
vi sz,order;
vvi adj;
void dfs_military(int u) {
    tin[u] = sz(order);
    order.pb(u);
    for (auto v : adj[u]) {
        dfs_military(v);
    }
    sz[u] = sz(order)-tin[u];
}
struct Query {
    int l,r,id;
};
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
vi vis;
vvi g;
vii inout;
vi val;
vi values;
struct segment_tree_arr{
    int n;
    vi value;
    segment_tree_arr(int n): value(4*n, 0), n(n){
        build(1, 1, n);
    }

    void build(int node, int l, int r){
        if(l == r) {
            value[node] = values[l];
            return;
        }
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
void eulert(int u) {
    t++;
    inout[u].first = t;
    vis[u]=1;
    for (int v : g[u]) {
        if (!vis[v]) {
            eulert(v);
        }
    }
    t++;
    inout[u].second = t;
}

int block_size;
vector<int> mos_algorithm(vi& a,vector<Query>& queries,int k) {
    int n=a.size();
    int q=queries.size();
    block_size=sqrt(n)+1;
    sort(all(queries),[&](Query x,Query y) {
        int block_x=x.l/block_size;
        int block_y=y.l/block_size;
        if(block_x!=block_y)
            return block_x<block_y;
        if(block_x%2==0)
            return x.r<y.r;
        return x.r>y.r;
    });
    vi ans(q);
    vi cnt(1<<20,0);
    int l=0,r=-1;
    int cur=0;
    auto add=[&](int i) {
        int x=a[i];
        cur+=cnt[x^k];
        cnt[x]++;
    };
    auto remove=[&](int i) {
        int x=a[i];
        cnt[x]--;
        cur-=cnt[x^k];
    };
    for(auto query:queries) {
        int ql=query.l;
        int qr=query.r;
        while(l>ql)
            add(--l);
        while(r<qr)
            add(++r);
        while(l<ql)
            remove(l++);
        while(r>qr)
            remove(r--);
        ans[query.id]=cur;
    }
    return ans;
}


void Path_Queries() {
    int n,q;
    cin>>n>>q;
    a.assign(n,0);
    visited.assign(n,0);
    euler.assign(2*n,0);
    tin.assign(n,-1);
    tout.assign(n,-1);
    rep(i,0,n) cin>>a[i];
    vvi adj(n);
    rep(i,0,n-1) {
        int u, v;
        cin>>u>>v;
        u--;
        v--;
        adj[u].pb(v);
        adj[v].pb(u);
    }
    euler_tour_val(adj,0);
    segment_tree_arr_euler tree(2*n);
    for (int i = 0; i < q; i++) {
        int type;
        cin>>type;
        if (type == 1) {
            int s,x;
            cin>>s>>x;
            tree.update(tin[s-1]+1,x-a[s-1]);
            tree.update(tout[s-1]+1,a[s-1]-x);
            a[s-1] = x;
        }
        else {
            int s;
            cin>>s;
            cout<<tree.query(1,tout[s-1]+1) + a[s-1]<<endl;
        }
    }
}
void Military_Problem() {
    int n,q;
    cin>>n>>q;
    adj.assign(n,{});
    tin.assign(n,-1);
    sz.assign(n,0);
    rep(i,1,n) {
        int p;
        cin>>p;
        p--;
        adj[p].pb(i);
    }
    dfs_military(0);
    rep(i,0,q) {
        int u,k;
        cin>>u>>k;
        u--;
        if (k>sz[u]) {
            cout<<-1<<endl;
        }
        else {
            cout<<order[tin[u]+k-1]+1<<endl;
        }
    }
}
void Little_Elephant_and_Array() {
    int n,m;
    cin>>n>>m;
    a.assign(n,0);
    rep(i,0,n) cin>>a[i];
    int block_size = sqrt(n)+1;
    vector<Query> queries(m);
    rep(i,0,m) {
        int l,r;
        cin>>l>>r;
        l--;
        r--;
        queries[i] = {l,r,i};
    }
    sort(all(queries), [&](Query x, Query y) {
        int block_x = x.l/block_size;
        int block_y = y.l/block_size;

        if (block_x != block_y)
            return block_x < block_y;

        if (block_x % 2 == 0)
            return x.r < y.r;

        return x.r > y.r;
    });

    vi cnt(n+1,0);
    vi ans(m);
    int l = 0,r = -1;
    int cur = 0;
    for (auto query : queries) {
        int ql = query.l;
        int qr = query.r;
        while (l>ql) {
            l--;
            int x = a[l];
            if (x<=n) {
                if (cnt[x]==x)
                    cur--;
                cnt[x]++;
                if (cnt[x]==x)
                    cur++;
            }
        }
        while (r<qr) {
            r++;
            int x = a[r];
            if (x<=n) {
                if (cnt[x]==x)
                    cur--;
                cnt[x]++;
                if (cnt[x]==x)
                    cur++;
            }
        }
        while (l<ql) {
            int x = a[l];
            if (x<=n) {
                if (cnt[x]==x)
                    cur--;
                cnt[x]--;
                if (cnt[x]==x)
                    cur++;
            }

            l++;
        }
        while (r>qr) {
            int x = a[r];
            if (x<=n) {
                if (cnt[x]==x)
                    cur--;
                cnt[x]--;
                if (cnt[x]==x)
                    cur++;
            }
            r--;
        }
        ans[query.id] = cur;
    }
    rep(i,0,m) {
        cout<<ans[i]<<endl;
    }
}
void Distance_Queries() {
    int n,q;
    cin>>n>>q;
    adj.assign(n, vi());
    rep(i,0,n-1) {
        int u,v;
        cin>>u>>v;
        u--; v--;
        adj[u].pb(v);
        adj[v].pb(u);
    }
    auto lif = lift(adj);
    for (int i=0;i<q;i++) {
        int u,v;
        cin>>u>>v;
        u--; v--;
        cout<<lif.d[u]+lif.d[v]-2*lif.d[lif.lca(u,v)]<<endl;
    }

}
void XOR_and_Favorite_Number() {
    int n,m,k;
    cin>>n>>m>>k;
    vi pre(n+1);
    pre[0]=0;
    for(int i=1;i<=n;i++) {
        int x;
        cin>>x;
        pre[i]=pre[i-1]^x;
    }
    vector<Query> queries(m);
    for(int i=0;i<m;i++) {
        int l,r;
        cin>>l>>r;
        queries[i]={l-1,r,i};
    }
    vi ans=mos_algorithm(pre,queries,k);
    for(int x:ans)
        cout<<x<<"\n";
}
void Subtree_Queries(){
    int n,q;
    t=0;
    cin>>n>>q;
    vis = vi(n+1,0);
    g.assign(n+1,vi());
    val = vi(n+1);
    inout = vii(n+1);
    for(int i=1;i<=n;i++) {
        cin>>val[i];
    }
    for(int i=1;i<=n-1;i++) {
        int a,b;
        cin>>a>>b;
        g[a].pb(b);
        g[b].pb(a);
    }
    eulert(1);
    values = vi(2*n+1,0);
    for(int i=1;i<=n;i++) {
        values[inout[i].first] = val[i];
        values[inout[i].second] = val[i];
    }
    segment_tree_arr st(2*n+1);
    for(int i=1;i<=q;i++) {
        int f,s;
        cin>>f>>s;
        if (f==1) {
            int x;
            cin>>x;
            st.update(inout[s].first,-val[s]+x);
            st.update(inout[s].second,-val[s]+x);
            val[s] = x;
        }
        else {
            cout<<st.query(inout[s].first,inout[s].second)/2<<"\n";
        }
    }
}
void Tree_Queries() {
    int n,m;
    cin>>n>>m;
    adj.assign(n+1,vi());
    rep(i,0,n-1) {
        int u,v;
        cin>>u>>v;
        u--; v--;
        adj[u].pb(v);
        adj[v].pb(u);
    }
    auto lif = lift(adj);
    rep(i,0,m) {
        int k;
        cin>>k;
        vi v(k);
        int deep = 0;
        rep(j,0,k) {
            cin>>v[j];
            v[j]--;
            if (v[j]!=0)
                v[j] = lif.p[v[j]];
            if (lif.d[v[j]] > lif.d[deep])
                deep = v[j];
        }
        int ok = 1;
        rep(j,0,k) {
            if (lif.lca(v[j],deep)!=v[j]) {
                ok = 0;
                break;
            }
        }
        cout<<(ok?"YES":"NO")<<endl;
    }
}
void Powerful_array() {
    int n,t;
    cin>>n>>t;
    a.assign(n,0);
    rep(i,0,n) cin>>a[i];
    int block_size = sqrt(n)+1;
    vector<Query> queries(t);
    rep(i,0,t) {
        int l,r;
        cin>>l>>r;
        l--; r--;
        queries[i] = {l,r,i};
    }
    sort(all(queries), [&](Query x, Query y) {
        int block_x = x.l/block_size;
        int block_y = y.l/block_size;
        if (block_x != block_y)
            return block_x < block_y;
        if (block_x%2==0)
            return x.r < y.r;
        return x.r > y.r;
    });
    vi cnt(1000001,0);
    vi ans(t);
    int l = 0,r = -1;
    int cur = 0;
    for (auto query : queries) {
        int ql = query.l;
        int qr = query.r;
        while (l>ql) {
            l--;
            int x = a[l];
            cur -= cnt[x]*cnt[x]*x;
            cnt[x]++;
            cur += cnt[x]*cnt[x]*x;
        }
        while (r<qr) {
            r++;
            int x = a[r];
            cur -= cnt[x]*cnt[x]*x;
            cnt[x]++;
            cur += cnt[x]*cnt[x]*x;
        }
        while (l<ql) {
            int x = a[l];
            cur -= cnt[x]*cnt[x]*x;
            cnt[x]--;
            cur += cnt[x]*cnt[x]*x;
            l++;
        }
        while (r>qr) {
            int x = a[r];
            cur -= cnt[x]*cnt[x]*x;
            cnt[x]--;
            cur += cnt[x]*cnt[x]*x;
            r--;
        }
        ans[query.id] = cur;
    }

    rep(i,0,t) {
        cout<<ans[i]<<endl;
    }
}

signed main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int ti = 1;
    //cin >> ti;
    while (ti--) {
        XOR_and_Favorite_Number();
    }
    return 0;
}

