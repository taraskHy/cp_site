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


signed H() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int t = 1;
    cin >> t;
    while (t--) {
        Ethan_and_the_Group_Project_From_Hell();
        cout<<"\n";
    }
    return 0;
}
