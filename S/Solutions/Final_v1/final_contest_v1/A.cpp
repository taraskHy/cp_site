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

signed A() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int t = 1;
    //cin >> t;
    while (t--) {
        Shalev_and_the_Infinity_Multiset();
        cout<<"\n";
    }
    return 0;
}
