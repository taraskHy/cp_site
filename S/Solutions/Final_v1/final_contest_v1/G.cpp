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

signed G() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int t = 1;
    cin >> t;
    while (t--) {
        Ethan_and_the_XORting_Hat();
        cout<<"\n";
    }
    return 0;
}
