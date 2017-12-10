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
    int l, r, fl = 0;
    char c = '.';
    double val;
};

double tn[max_n];
node v[max_n];
vector<int> vv;
pair<double, string> ans;

bool get_less(int ind, double sum, int n) {
    //cout << "!@#!@#     " << ind << " " << sum << " " << n << endl;
    vector<double> a, b;
    a.push_back(1);
    b.push_back(1);
    int curr = v[ind].l;
    //cout << curr << endl;
    while (curr > 1 && v[curr].fl == 0) {
        a.push_back(v[curr].val);
        a.back() *= a[a.size() - 2];
        curr = v[curr].l;
    }
    curr = v[ind].r;
    //cout << curr << endl;
    while (curr <= n && v[curr].fl == 0) {
        //cout << "!!! " << curr << endl;
        b.push_back(v[curr].val);
        b.back() *= b[b.size() - 2];
        curr = v[curr].r;
    }
    int inda = 0, indb = 0;
    double val = inf;
    for (int i = 0; i < a.size(); ++i) {
        for (int q = 0; q < b.size(); ++q) {
            //cout << fabs(sum) << " " << fabs(a[i] * b[q] * v[ind].val) << " " << fabs(val) << endl;
            double nxt = a[i] * b[q] * v[ind].val;
            if (fabs(fabs(nxt) - fabs(sum)) < fabs(fabs(val) - fabs(sum))) {
                val = a[i] * b[q] * v[ind].val;
                inda = i;
                indb = q;
            }
        }
    }

    if (inda == 0 && indb == 0) {
        return false;
    }
    //cout << "## " << inda << " " << indb << endl;

    for (int i = 0; i < indb; ++i) {
        v[v[ind].r].c = '*';
        v[ind].fl |= v[v[ind].r].fl;
        vv.erase(std::remove(vv.begin(), vv.end(), v[ind].r), vv.end());
        v[ind].r = v[v[ind].r].r;
        v[v[ind].r].l = ind;
    }
    for (int i = 0; i < inda; ++i) {
        v[ind].c = '*';
        v[v[ind].l].fl |= v[ind].fl;
        vv.erase(std::remove(vv.begin(), vv.end(), ind), vv.end());
        v[v[ind].l].r = v[ind].r;
        v[v[ind].r].l = v[ind].l;
        ind = v[ind].l;
    }
    v[ind].val = val;
    return true;
}

void get(int n, double sum) {
    /*cout << n << " " << sum << endl;
    for (int i = 2; i <= n; ++i) {
        cout << v[i].c;
    }cout << endl;
    for (int i = 2; i <= n; ++i) {
        cout << v[i].fl;
    }cout << endl;*/
    if (vv.size() == 0) {
        return;
    }
    bool fl = true;
    while (fl) {
        fl = false;
        for (int i = 0; i < vv.size(); ++i) {
            if (fabs(v[vv[i]].val) > fabs(sum) * 1.2) {
                if (get_less(vv[i], sum, n)) {
                    fl = true;
                    break;
                }
            }
        }
    }
    int ind;
    double val = 0;
    for (int i = 0; i < vv.size(); ++i) {
        if (fabs(val) < fabs(v[vv[i]].val)) {
            val = v[vv[i]].val;
            ind = vv[i];
        }
    }
    if (fabs(sum + val) < fabs(sum)) {
        v[ind].c = '-';
        sum += val;
    } else {
        v[ind].c = '+';
        sum -= val;
    }
    v[v[ind].l].r = v[ind].r;
    v[v[ind].r].l = v[ind].l;
    if (v[v[ind].r].fl == 0)
        v[v[ind].r].fl = 1;
    if (v[v[ind].r].fl == 3)
        v[v[ind].r].fl = 2;
    if (v[v[ind].l].fl == 0)
        v[v[ind].l].fl = 3;
    if (v[v[ind].l].fl == 1)
        v[v[ind].l].fl = 2;
    vv.erase(std::remove(vv.begin(), vv.end(), ind), vv.end());
    get(n, sum);
}

void clr(int n) {
    vv.clear();
    for (int i = 2; i <= n; ++i) {
        vv.push_back(i);
        v[i].l = i - 1;
        v[i].r = i + 1;
        v[i].val = tn[i];
        v[i].c = '.';
        v[i].fl = 0;
    }
    v[1].r = 2;
    v[1].l = 0;
    v[1].c = '+';
    v[2].l = 0;
}

double calc(int n) {
    double last = tn[1], res = 0;
    //for (int i = 2; i <= n; ++i) {
    //    cout << v[i].c;
    //}cout << endl;
    for (int i = 2; i <= n; ++i) {
        if (v[i].c != '*') {
            res += last;
            //cout << last << " " << v[i].c << " ";
            last = tn[i];
            if (v[i].c == '-') last *= -1;
        } else {
            last *= tn[i];
        }
    }
    //cout << last << endl;
    res += last;
    return res;
}

int main() {
    //freopen("input.txt", "r", stdin);
    //freopen("output.txt", "w", stdout);
    ofstream out("outcnt.txt");
    for (int i = 1; i < max_n; ++i) {
        tn[i] = tan(i);
        //if (i < 32) cout << i << " " << tn[i] << endl;
    }
    int cnt = 0;
    for (int i = 5; i < max_n; ++i) {
        clr(i);
        get(i, i - tn[1]);
        ans.S = "";
        for (int q = 2; q <= i; ++q) {
            ans.S += v[q].c;
        }

        if (fabs(i - calc(i)) > 6) cout << i << " " << i - calc(i) << endl, ++cnt;
        out << i << " " << ans.S << endl;
        //if (i > 30) return 0;
        //out << ans.F << endl;
    }
    cout << cnt << endl;
    return 0;
}
