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


signed B() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int t = 1;
    //cin >> t;
    while (t--) {
        Shalevs_Minecraft_Industrial_Revolution();
        cout<<"\n";
    }
    return 0;
}
