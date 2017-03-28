#include <iostream>
#include <cstdio>

using namespace std;

const int MAXN = 110000;

struct _node;
typedef _node* node;

struct _node{
    static node null;
    node l,r,p;
    int size;
    bool rev;
    node pp;
    _node(){
        l = r = p = pp = _node::null;
        size = 1;
        rev = 0;
    }
    _node(void*){
        l = r = p = pp = this;
        size = rev = 0;
    }
    void push(){
        if (rev){
            l->rev ^= 1;
            r->rev ^= 1;
            rev = 0;
            swap(l,r);
        }
    }
    void update(){
        size = (this != null) + l->size + r->size;
        l->p = r->p = this;
    }
};

node _node::null = new _node(NULL);

_node mem[MAXN];
node v2n[MAXN];

void rotate(node v){
    node u = v->p;
    if (v == u->l){
       u->l = v->r;
       v->r = u;
    }
    else {
       u->r = v->l;
       v->l = u;
    }
    swap(u->p,v->p);
    swap(v->pp,u->pp);
    if (v->p != _node::null){
        if (v->p->r == u)
            v->p->r = v;
        else
            v->p->l = v;
    }
    u->update();
    v->update();
}

void bigRotate(node v){
    v->p->p->push();
    v->p->push();
    v->push();
    if (v->p->p == _node::null)
        rotate(v);
    else if ((v->p->l == v) ^ (v->p->p->r == v->p))
        rotate(v->p), rotate(v);
    else
        rotate(v), rotate(v);
}

inline void Splay(node v){
     while (v->p != _node::null)
        bigRotate(v);
}

inline void splitAfter(node v){
    v->push();
    Splay(v);
    v->r->p = _node::null;
    v->r->pp = v;
    v->r = _node::null;
    v->update();
}

void expose(int x){
   node v = v2n[x];
   splitAfter(v);
   while (v->pp != _node::null){
       splitAfter(v->pp);
       v->pp->r = v;
       v->pp->update();
       v = v->pp;
       v->r->pp = _node::null;
   }
   Splay(v2n[x]);
}

inline void makeRoot(int x){
    expose(x);
    v2n[x]->rev ^= 1;
}

inline void link(int x,int y){
    makeRoot(x);
    makeRoot(y);
    v2n[x]->pp = v2n[y];
}

inline void cut(int x,int y){
    expose(x);
    Splay(v2n[y]);
    if (v2n[y]->pp != v2n[x]){
        swap(x,y);
        expose(x);
        Splay(v2n[y]);
    }
    v2n[y]->pp = _node::null;
}

inline int get(int x,int y){
    if (x == y)
        return 0;
    makeRoot(x);
    expose(y);
    expose(x);
    Splay(v2n[y]);
    if (v2n[y]->pp != v2n[x]) return -1;
    return v2n[y]->size;
}

int main(){
    //freopen("linkcut.in","r",stdin);
    //freopen("linkcut.out","w",stdout);

    int n,m;
    scanf("%d %d",&n,&m);

    for (int i = 0; i < n; i++)
        v2n[i] = &mem[i];

    for (int i = 0; i < m; i++){
        int a,b;
        if (scanf(" link %d %d",&a,&b) == 2)
            link(a-1,b-1);
        else if (scanf(" cut %d %d",&a,&b) == 2)
            cut(a-1,b-1);
        else if (scanf(" get %d %d",&a,&b) == 2)
            printf("%d\n",get(a-1,b-1));
    }
    return 0;
}
