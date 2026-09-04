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

signed C() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int t = 1;
    //cin >> t;
    while (t--) {
        Shalev_Forgot_to_Shut_Down_the_Server();
        cout<<"\n";
    }
    return 0;
}
