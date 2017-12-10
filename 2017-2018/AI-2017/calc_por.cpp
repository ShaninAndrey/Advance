#include <bits/stdc++.h>

#define F first
#define S second
#define x1 privet1
#define x2 privet2
#define y1 privet3
#define y2 privet4
#define left privet6
#define prev ppppp

using namespace std;
typedef long long ll;

const int max_n = 366, log_n = 32, max_m = 111, mod = 1000000007, inf = 1000111222;

struct node {
    int ind, cnt = 1;
    double val;
};

const int binpval = 20, checkcalc = 150;
int check_fail = 0;
double tn[max_n];


vector<node> clr(int l, int r) {
    vector<node> ans;
    for (int i = l; i <= r; ++i) {
        ans.push_back(node());
        ans.back().val = tn[i];
        ans.back().ind = i;
    }
    return ans;
}

double calc(string ans) {
    double last = tn[1], res = 0;
    //for (int i = 2; i <= n; ++i) {
    //    cout << v[i].c;
    // }cout << endl;
    for (int i = 0; i < ans.size(); ++i) {
        if (ans[i] != '*') {
            res += last;
            //cout << last << " " << v[i].c << " ";
            last = tn[i + 2];
            if (ans[i] == '-') last *= -1;
        } else {
            last *= tn[i + 2];
        }
    }
    //cout << last << endl;
    res += last;
    return res;
}

bool _check(vector<node> &v, double a, double b) {
    if (v.size() < 1) return false;
    vector<double> vv;
    for (int i = 0; i < v.size(); ++i) {
        vv.push_back(fabs(v[i].val));
    }
    sort(vv.begin(), vv.end());
    double sum = vv[0];
    if (sum > a) return false;
    for (int i = 1; i < vv.size(); ++i) {
        // cout << sum << " " << b << " " << vv[i] << " " << vv.size() << endl;
        if (sum * 2 < vv[i]) {
            // cout << sum << " " << b << endl << endl;
            return sum >= b;
        }
        sum += vv[i];
    }
    // cout << endl;
    return sum >= b;
}

bool cmp(vector<node> &a, vector<node> &b) {
    return a.size() > b.size();
}

vector<vector<node> > get_less(vector<node> &v, double mn, double val) {
    vector<vector<node> > a;

    for (int i = 0; i < v.size(); ++i) {
        double sum = 1;
        int cnt = 0;
        for (int q = i; q < v.size(); ++q) {
            sum *= v[q].val;
            cnt += v[q].cnt;
            vector<node> vv;
            for (int w = 0; w < i; ++w) vv.push_back(v[w]);
            vv.push_back(v[i]);
            vv[i].val = sum;
            vv[i].cnt = cnt;
            for (int w = q + 1; w < v.size(); ++w) vv.push_back(v[w]);

            if (_check(vv, mn, val)) {
                if (vv.size() == v.size())
                    exit(123123123);
                a.push_back(vv);
            }
        }
    }
    sort(a.begin(), a.end(), cmp);
    return a;
}

vector<node> check(vector<node> v, double mn, double mx) {
    if (check_fail > checkcalc)
        //exit(1111);
        return vector<node>();
    vector<pair<double, int> > vv;
    for (int i = 0; i < v.size(); ++i) {
        vv.push_back(make_pair(fabs(v[i].val), i));
    }
    sort(vv.begin(), vv.end());

    double sum = vv[0].F;
    if (sum > mn) {
        vector<vector<node> > a = get_less(v, mn, -inf);
        for (int i = 0; i < a.size(); ++i) {
            vector<node> aaa = check(a[i], mn, mx);
            if (aaa.size() > 0)
                return aaa;
        }
    }
    bool fl = false;
    for (int i = 1; i < vv.size(); ++i) {
        // cout << "!!!   " << i << " " << sum << " " << vv[i].F << endl;
        if (sum * 2 < vv[i].F) {
            fl = true;
            vector<vector<node> > a = get_less(v, mn, vv[i].F);
            for (int q = 0; q < a.size(); ++q) {
                // for (int w = 0; w < a[q].size(); ++w) {
                //    cout << a[q][w].val << " ";
                // }cout << endl;
                //++check_fail;
                //if (check_fail > checkcalc) exit(1212);
                // cout << check_fail << endl;
                vector<node> aaa = check(a[q], mn, mx);
                if (aaa.size() > 0)
                    return aaa;
            }
        }
        sum += vv[i].F;
    }
    if (!fl) {
        return v;
    }
    ++check_fail;
    return vector<node>();
}

string config(vector<node> &v, double sum, int n) {
    vector<pair<double, int> > vv;
    vector<int> pp;
    string ans(n, '*');
    for (int i = 0; i < v.size(); ++i) {
        pp.push_back(v[i].cnt);
        if (pp.size() > 1)
            pp.back() += pp[pp.size() - 2];
        vv.push_back(make_pair(fabs(v[i].val), i));
    }
    sort(vv.begin(), vv.end());
    for (int i = vv.size() - 1; i >= 0; --i) {
        double val = v[vv[i].S].val;
        if (fabs(sum + val) < fabs(sum - val)) {
            ans[pp[vv[i].S] - v[vv[i].S].cnt] = '-';
        } else {
            ans[pp[vv[i].S] - v[vv[i].S].cnt] = '+';
        }
    }
    return ans;
}

string binp(int l, int r, double sum) {
    vector<node> v = clr(l, r);
    double ll = 0.0000001, rr = 100.0;
    for (int e = 0; e < binpval; ++e) {
        double mid = (ll + rr) / 2;
        check_fail = 0;
        vector<node> a = check(v, mid, sum);
        if (a.size()) {
            rr = mid;
        } else {
            ll = mid;
        }
    }
    v = check(v, rr, sum);
    if (v.size() < 1) return "";
    return config(v, sum, r - l + 1);
}

string get(int n) {
    double sum = 1.0;
    string res = "";
    pair<double, string> ans = make_pair(inf, "");
    for (int i = 1; i <= n; ++i) {
        sum *= tn[i];
        string tmp = i == n ? "" : binp(i + 1, n, n - sum);
        if (tmp.length() >= 1) {
            // cout << i << " " << calc(res + tmp) << " " << res + tmp << endl;
            ans = min(ans, make_pair(fabs(n - calc(res + tmp)), res + tmp));
        }
        res += '*';
    }
    return ans.S;
}

int main() {
    //freopen("input.txt", "r", stdin);
    //freopen("output.txt", "w", stdout);
    ofstream out("outpow.txt");
    for (int i = 1; i < max_n; ++i) {
        tn[i] = tan(i);
        //if (i < 32) cout << i << " " << tn[i] << endl;
    }
    int cnt = 0;
    for (int i = 41; i < max_n; ++i) {
        string ans = get(i);
        cout << i << " " << i - calc(ans) << endl, ++cnt;
        out << i << " " << ans << endl;
        //if (i > 30) return 0;
        //out << ans.F << endl;
    }
    cout << cnt << endl;
    return 0;
}
