#include <bits/stdc++.h>
#include <vector>
#include <algorithm>

typedef long long ll;

const long long max_n = 100011;

/*
* REWRITE:
* node::merge
* node::push_merge
* get
*/
struct linkCutTree {
    struct node {
        node *pp, *l, *r, *p;
        int sz, val, sum, push_val;
        bool rev, p_val;

        node() {
            pp = l = r = p = NULL;
            sz = 1;
            val = sum = push_val = 0;
            rev = p_val = false;
        }
        explicit node(const int value) {
            pp = l = r = p = NULL;
            sz = 1;
            val = sum = value;
            push_val = 0;
            rev = p_val = false;
        }
        void merge() {
            sum = val;
            if (l) sum = std::max(sum, l->sum);
            if (r) sum = std::max(sum, r->sum);
        }
        void push_merge(const int new_val) {
            val += new_val;
            merge();
        }
        void update_value(const int new_val) {
            val += new_val;
            merge();
        }
        void update() {
            sz = 1;
            if (l) {
                sz += l->sz;
                l->p = this;
            }
            if (r) {
                sz += r->sz;
                r->p = this;
            }
            merge();
        }
        void push() {
            if (rev) {
                if (l) l->rev ^= 1;
                if (r) r->rev ^= 1;
                std::swap(l, r);
                rev = 0;
            }
            if (p_val) {
                if (l) {
                    l->push_merge(push_val);
                    l->p_val = true;
                    l->merge();
                }
                if (r) {
                    r->push_merge(push_val);
                    r->p_val = true;
                    r->merge();
                }
                p_val = 0;
            }
        }

        void rotate() {
            node *t = p;
            if (t->l == this) {
                t->l = r;
                r = t;
            } else {
                t->r = l;
                l = t;
            }
            std::swap(p, t->p);
            std::swap(pp, t->pp);
            if (p) {
                if (p->l == t) {
                    p->l = this;
                } else {
                    p->r = this;
                }
            }
            t->update();
            update();
        }
        void bigRotate() {
            if (p->p) p->p->push();
            p->push();
            push();
            if (p->p && p->l == this ^ p->p->r == p) {
                p->rotate();
            } else if (p->p) {
                rotate();
            }
            rotate();
        }
        inline void splay() {
            if (!p) push();
            while (p) {
                bigRotate();
            }
        }
    };

    int num;
    std::vector<node*> vv;

    linkCutTree() { }
    explicit linkCutTree(const int max_n) {
        resize(max_n);
    }
    explicit linkCutTree(const std::vector<int> &values) {
        num = values.size();
        vv.resize(values.size());
        for (int i = 0; i < values.size(); ++i) {
            vv[i] = new node(values[i]);
        }
    }
    void resize(const int max_n) {
        num = max_n;
        vv.resize(num);
        for (int i = 0; i < num; ++i) {
            vv[i] = new node();
        }
    }

    inline void splitAfter(node *vv) {
        vv->splay();
        if (!vv->r) return;
        vv->r->p = NULL;
        vv->r->pp = vv;
        vv->r = NULL;
        vv->update();
    }
    void access(const int xx) {
        splitAfter(vv[xx]);
        node *t = vv[xx];
        while (t->pp) {
            splitAfter(t->pp);
            t->pp->r = t;
            t->pp->update();
            t = t->p;
            t->r->pp = NULL;
        }
        vv[xx]->splay();
    }
    inline void makeRoot(const int xx) {
        access(xx);
        vv[xx]->rev ^= 1;
    }

    inline void link(const int xx, const int yy) {
        makeRoot(xx);
        makeRoot(yy);
        vv[xx]->pp = vv[yy];
        access(xx);
    }
    inline void cut(const int xx, const int yy) {
        makeRoot(xx);
        access(xx);
        vv[yy]->splay();
        vv[yy]->pp = NULL;
    }

    inline void update_val(const int xx, const int val) {
        vv[xx]->splay();
        vv[xx]->update_value(val);
    }
    inline void update_values(const int xx, const int yy, const int val) {
        makeRoot(xx);
        access(yy);
        vv[yy]->push_val = val;
        vv[yy]->p_val = true;
    }
    inline int get(const int xx, const int yy) {
        makeRoot(xx);
        access(yy);
        return vv[yy]->sum;
    }
};

int mm, n;
linkCutTree LCT;

int main() {
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
    scanf("%d", &n);
    LCT.resize(n);
    for (int i = 0; i < n - 1; ++i) {
        int aa, bb;
        scanf("%d%d", &aa, &bb);
        LCT.link(aa - 1, bb - 1);
    }
    scanf("%d", &mm);
    for (int i = 0; i < mm; ++i) {
        char arr[2];
        int aa, bb;
        scanf("%s%d%d", arr, &aa, &bb);
        if (arr[0] == 'I') {
            LCT.update_val(aa - 1, bb);
        } else {
            printf("%d\n", LCT.get(aa - 1, bb - 1));
        }
    }
    return 0;
}
