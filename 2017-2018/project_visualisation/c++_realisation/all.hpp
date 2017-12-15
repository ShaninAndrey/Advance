#include <cmath>
// #include "../src/MemoryPool/C-11/MemoryPool.h"
#include <iostream>
#include <vector>
#include <set>
#include <iostream>
#include <sstream>
#include <iostream>
#include <math.h>
#include <sstream>
#include <vector>
#include <forward_list>

using std::cout;
using std::cin;
using std::endl;


#define EPSILON 1e-9
#define INV_EPSILON (1.0 / EPSILON);


struct Edge;
struct HalfEdge;
struct BoundingBox;
struct BeachSection;
struct CircleEvent;
struct CircleEventQueue;
struct cellBoundingBox;
struct Site;
struct Cell;



inline bool eq_withEpsilon(double a, double b) { return abs(a - b) < EPSILON; }
inline bool gt_withEpsilon(double a, double b) { return a - b > EPSILON; }
inline bool gteq_withEpsilon(double a, double b) { return b - a < EPSILON; }
inline bool lt_withEpsilon(double a, double b) { return b - a > EPSILON; }
inline bool lteq_withEpsilon(double a, double b) { return a - b < EPSILON; }


template <typename T>
struct treeNode {
    T data;
    treeNode<T>* left;
    treeNode<T>* right;
    treeNode<T>* parent;
    bool red;

    //cached for speed
    treeNode<T>* prev;
    treeNode<T>* next;

    treeNode() : left(NULL), right(NULL), parent(NULL),
                 red(false), prev(NULL), next(NULL) {};
};

template <typename T>
class RBTree {
 public:
    RBTree() : root(NULL) {};
    ~RBTree() {};

    treeNode<T>* insertSuccessor(treeNode<T>* node, T& successorData) {
        treeNode<T>* successor = new treeNode<T>();
        successor->data = successorData;

        treeNode<T>* parent = NULL;

        if (node) {
            successor->prev = node;
            successor->next = node->next;
            if (node->next) {
                node->next->prev = successor;
            }
            node->next = successor;
            if (node->right) {
                node = getFirst(node->right);
                node->left = successor;
            }
            else {
                node->right = successor;
            }
            parent = node;
        }
            // if node is null, successor must be inserted
            // to the left-most part of the tree
        else if (root) {
            node = getFirst(root);
            successor->prev = NULL;
            successor->next = node;
            node->prev = successor;
            node->left = successor;
            parent = node;
        }
        else {
            successor->prev = successor->next = NULL;
            root = successor;
            parent = NULL;
        }
        successor->left = successor->right = NULL;
        successor->parent = parent;
        successor->red = true;

        // Fixup the modified tree by recoloring nodes and performing rotations
        // (2 at most) so the red-black tree properties are preserved.
        treeNode<T>* grandma;
        treeNode<T>* aunt;
        node = successor;
        while (parent && parent->red) {
            grandma = parent->parent;
            if (parent == grandma->left) {
                aunt = grandma->right;
                if (aunt && aunt->red) {
                    parent->red = aunt->red = false;
                    grandma->red = true;
                    node = grandma;
                }
                else {
                    if (node == parent->right) {
                        rotateLeft(parent);
                        node = parent;
                        parent = node->parent;
                    }
                    parent->red = false;
                    grandma->red = true;
                    rotateRight(grandma);
                }
            }
            else {
                aunt = grandma->left;
                if (aunt && aunt->red) {
                    parent->red = aunt->red = false;
                    grandma->red = true;
                    node = grandma;
                }
                else {
                    if (node == parent->left) {
                        rotateRight(parent);
                        node = parent;
                        parent = node->parent;
                    }
                    parent->red = false;
                    grandma->red = true;
                    rotateLeft(grandma);
                }
            }
            parent = node->parent;
        }
        root->red = false;
        return successor;
    }
    void removeNode(treeNode<T>* node) {
        if (node->next) {
            node->next->prev = node->prev;
        }
        if (node->prev) {
            node->prev->next = node->next;
        }

        treeNode<T>* original = node;
        treeNode<T>* parent = node->parent;
        treeNode<T>* left = node->left;
        treeNode<T>* right = node->right;
        treeNode<T>* next = NULL;

        if (!left) {
            next = right;
        }
        else if (!right) {
            next = left;
        }
        else {
            next = getFirst(right);
        }
        if (parent) {
            if (parent->left == node) {
                parent->left = next;
            }
            else {
                parent->right = next;
            }
        }
        else {
            root = next;
        }
        //enforce red-black rules
        bool red;
        if (left && right) {
            red = next->red;
            next->red = node->red;
            next->left = left;
            left->parent = next;
            if (next != right) {
                parent = next->parent;
                next->parent = node->parent;
                node = next->right;
                parent->left = node;
                next->right = right;
                right->parent = next;
            }
            else {
                next->parent = parent;
                parent = next;
                node = next->right;
            }
        }
        else {
            red = node->red;
            node = next;
        }
        // 'node' is now the sole successor's child and 'parent' its
        // new parent (since the successor can have been moved)
        if (node) {
            node->parent = parent;
        }
        // the 'easy' cases
        if (red) {
            delete original;
            return;
        }
        if (node && node->red) {
            node->red = false;
            delete original;
            return;
        }
        // the other cases
        treeNode<T>* sibling = NULL;
        do {
            if (node == root) {
                break;
            }
            if (node == parent->left) {
                sibling = parent->right;
                if (sibling->red) {
                    sibling->red = false;
                    parent->red = true;
                    rotateLeft(parent);
                    sibling = parent->right;
                }
                if ((sibling->left && sibling->left->red) || (sibling->right && sibling->right->red)) {
                    if (!sibling->right || !sibling->right->red) {
                        sibling->left->red = false;
                        sibling->red = true;
                        rotateRight(sibling);
                        sibling = parent->right;
                    }
                    sibling->red = parent->red;
                    parent->red = sibling->right->red = false;
                    rotateLeft(parent);
                    node = root;
                    break;
                }
            }
            else {
                sibling = parent->left;
                if (sibling->red) {
                    sibling->red = false;
                    parent->red = true;
                    rotateRight(parent);
                    sibling = parent->left;
                }
                if ((sibling->left && sibling->left->red) || (sibling->right && sibling->right->red)) {
                    if (!sibling->left || !sibling->left->red) {
                        sibling->right->red = false;
                        sibling->red = true;
                        rotateLeft(sibling);
                        sibling = parent->left;
                    }
                    sibling->red = parent->red;
                    parent->red = sibling->left->red = false;
                    rotateRight(parent);
                    node = root;
                    break;
                }
            }
            sibling->red = true;
            node = parent;
            parent = parent->parent;
        } while (!node->red);
        if (node) {
            node->red = false;
        }

        delete original;
    }
    inline treeNode<T>* getFirst(treeNode<T>* node) {
        while (node->left) {
            node = node->left;
        }
        return node;
    }
    inline treeNode<T>* getLast(treeNode<T>* node) {
        while (node->right) {
            node = node->right;
        }
        return node;
    }

    treeNode<T>* getRoot() { return root; };

    void print() {
        treeNode<T>* node = getFirst(root);

        while (node) {
            std::cout << node->data << std::endl;
            node = node->next;
        }
    }
 private:
    treeNode<T>* root;

    void rotateLeft(treeNode<T>* node) {
        treeNode<T>* p = node;
        treeNode<T>* q = node->right; //can't be null
        treeNode<T>* parent = p->parent;

        if (parent) {
            if (parent->left == p) {
                parent->left = q;
            }
            else {
                parent->right = q;
            }
        }
        else {
            root = q;
        }
        q->parent = parent;
        p->parent = q;
        p->right = q->left;
        if (p->right) {
            p->right->parent = p;
        }
        q->left = p;
    }
    void rotateRight(treeNode<T>* node) {
        treeNode<T>* p = node;
        treeNode<T>* q = node->left; //can't be null
        treeNode<T>* parent = p->parent;

        if (parent) {
            if (parent->left == p) {
                parent->left = q;
            }
            else {
                parent->right = q;
            }
        }
        else {
            root = q;
        }
        q->parent = parent;
        p->parent = q;
        p->left = q->right;
        if (p->left) {
            p->left->parent = p;
        }
        q->right = p;
    }
};





class Vector2 {
 public:
    Vector2() : x(0), y(0) {}
    Vector2(const Vector2& v) : x(v[0]), y(v[1]) {}
    Vector2(double _x, double _y) : x(_x), y(_y) {}

    Vector2& operator=(const Vector2& a) {
        x = a[0];
        y = a[1];
        return *this;
    }
    const double& operator[](int n) const {
        return (&x)[n];
    }
    double& operator[](int n) {
        return (&x)[n];
    }

    Vector2& operator+=(const Vector2& a) {
        x += a[0];
        y += a[1];
        return *this;
    }
    Vector2& operator-=(const Vector2& a) {
        x -= a[0];
        y -= a[1];
        return *this;
    }
    Vector2& operator*=(double s) {
        x *= s;
        y *= s;
        return *this;
    }

    Vector2 operator-() const {
        return Vector2(-x, -y);
    }
    Vector2 operator+() const {
        return *this;
    }
    Vector2 operator+(const Vector2 &v) const {
        return Vector2( x + v.x, y + v.y);
    }
    Vector2 operator-(const Vector2 &v) const {
        return Vector2( x - v.x, y - v.y);
    }
    Vector2 operator/(const double s) const {
        return Vector2( x / s, y / s);
    }
    Vector2 operator*(const double s) const {
        return Vector2( x * s, y * s);
    }
// Dot Product
    double operator*(const Vector2 &v) const {
        return x * v.x + y * v.y;
    }

    double length() const {
        return (double) sqrt(x * x + y * y);
    }
    double lengthSquared() const {
        return x * x + y * y;
    }
    void normalize() {
        double s = 1.0 / (double) sqrt(x * x + y * y);
        x *= s;
        y *= s;
    }
    bool operator==(const Vector2 &v) const {
        return x == v.x && y == v.y;
    }
    bool operator!=(const Vector2 &v) const {
        return x != v.x || y != v.y;
    }
    void print() const {
        std::cout << x << " " << y << "\n";
    }

    static inline double signedAngle(Vector2 v1, Vector2 v2){
        double angle = atan2(v2[1], v2[0]) - atan2(v1[1], v1[0]);
        if (angle < 0) angle += 2 * M_PI;

        return angle;
    };

    double x, y;
};
class Point2{
 public:
    double x, y;

    Point2() : x(0), y(0) {}
    Point2(const Point2& p) : x(p[0]), y(p[1]) {}
    Point2(double _x, double _y) : x(_x), y(_y) {}

    Point2& operator=(const Point2& a) {
        x = a[0];
        y = a[1];
        return *this;
    }

    const double& operator[](int n) const {
        return (&x)[n];
    }
    double& operator[](int n) {
        return (&x)[n];
    }

    Point2& operator+=(const Vector2& v) {
        x += v[0];
        y += v[1];
        return *this;
    }
    Point2& operator-=(const Vector2& v) {
        x -= v[0];
        y -= v[1];
        return *this;
    }
    Point2& operator*=(double s) {
        x *= s;
        y *= s;
        return *this;
    }

    Vector2 operator-(const Point2 & p) const {
        return Vector2(x - p.x, y - p.y);
    }
    Point2 operator+(const Vector2 & v) const {
        return Point2(x + v[0], y + v[1]);
    }
    Point2 operator-(const Vector2 & v) const {
        return Point2(x - v[0], y - v[1]);
    }

    double distanceTo(const Point2& p) const {
        return sqrt((p[0] - x) * (p[0] - x) +
            (p[1] - y) * (p[1] - y));
    }
    double distanceToSquared(const Point2& p) const {
        return ((p[0] - x) * (p[0] - x) +
            (p[1] - y) * (p[1] - y));
    }
    double distanceFromOrigin() const {
        return sqrt(x * x + y * y);
    }
    double distanceFromOriginSquared() const {
        return x * x + y * y;
    }

    bool operator==( const Point2 &p ) const {
        return x == p.x && y == p.y;
    }
    bool operator!=( const Point2 &p ) const {
        return x != p.x || y != p.y;
    }

    void print() const {
        std::cout << x << " " << y << "\n";
    }
};

inline Point2 lerp( const Point2& p0, const Point2& p1, double dT ){
    const double dTMinus = 1.0 - dT;
    return Point2( dTMinus * p0[0] + dT * p1[0], dTMinus * p0[1] + dT * p1[1]);
}
inline std::ostream& operator<<(std::ostream& os, const Point2& p){
    os << "(" << p[0] << ", " << p[1] << ")";
    return os;
}
inline Vector2 operator*(const double s, const Vector2 &v) {
    return Vector2( v[0] * s, v[1] * s);
}
inline double dot(const Vector2 &w, const Vector2 &v) {
    return w * v;
}
inline double length(const Vector2 &v) {
    return v.length();
}
inline Vector2 unit(const Vector2 &v) {
    const double len = v.length();
    return v / len;
}
inline std::ostream& operator<<(std::ostream& os, const Vector2& v) {
    os << "(" << v[0] << ", " << v[1] << ")";
    return os;
}






struct Site {
    Point2 p;
    Cell* cell;

    Site() {};
    Site(Point2 _p, Cell* _cell) : p(_p), cell(_cell) {};
};
struct Edge {
    Site* lSite;
    Site* rSite;
    Point2* vertA;
    Point2* vertB;

    Edge() : lSite(nullptr), rSite(nullptr), vertA(nullptr), vertB(nullptr) {};
    Edge(Site* _lSite, Site* _rSite) : lSite(_lSite), rSite(_rSite), vertA(nullptr), vertB(nullptr) {};
    Edge(Site* lS, Site* rS, Point2* vA, Point2* vB) : lSite(lS), rSite(rS), vertA(vA), vertB(vB) {};

    void setStartPoint(Site* _lSite, Site* _rSite, Point2* vertex) {
        if (!vertA && !vertB) {
            vertA = vertex;
            lSite = _lSite;
            rSite = _rSite;
        }
        else if (lSite == _rSite) {
            vertB = vertex;
        }
        else {
            vertA = vertex;
        }
    }
    void setEndPoint(Site* _lSite, Site* _rSite, Point2* vertex) {
        setStartPoint(_rSite, _lSite, vertex);
    }
};
struct HalfEdge {
    Site* site;
    Edge* edge;
    double angle;

    HalfEdge() : site(nullptr), edge(nullptr) {};
    HalfEdge(Edge* e, Site* lSite, Site* rSite) {
        site = lSite;
        edge = e;
        // 'angle' is a value to be used for properly sorting the
        // halfsegments counterclockwise. By convention, we will
        // use the angle of the line defined by the 'site to the left'
        // to the 'site to the right'.
        // However, border edges have no 'site to the right': thus we
        // use the angle of line perpendicular to the halfsegment (the
        // edge should have both end points defined in such case.)
        if (rSite) {
            angle = atan2(rSite->p.y - lSite->p.y, rSite->p.x - lSite->p.x);
        }
        else {
            Point2& va = *(e->vertA);
            Point2& vb = *(e->vertB);

            angle = (e->lSite == lSite) ? atan2(vb.x - va.x, va.y - vb.y) : atan2(va.x - vb.x, vb.y - va.y);
        }
    }

    inline Point2* startPoint() {
        return (edge->lSite == site) ? edge->vertA : edge->vertB;
    }
    inline Point2* endPoint() {
        return (edge->lSite == site) ? edge->vertB : edge->vertA;
    }
};
struct BoundingBox {
    double xL;
    double xR;
    double yB;
    double yT;

    BoundingBox() {};
    BoundingBox(double xmin, double xmax, double ymin, double ymax) :
        xL(xmin), xR(xmax), yB(ymin), yT(ymax) {};
};
struct BeachSection {
    Site* site;
    Edge* edge;
    treeNode<CircleEvent>* circleEvent;

    BeachSection() : site(nullptr), edge(nullptr), circleEvent(nullptr) {};
    ~BeachSection() {};
    BeachSection(Site* _site) : site(_site), edge(nullptr), circleEvent(nullptr) {};
};
struct CircleEvent {
    Site* site;
    double x;
    double y;
    double yCenter;
    treeNode<BeachSection>* beachSection;

    CircleEvent() {};
    ~CircleEvent() {};
    CircleEvent(Site* _site, double _x, double _y, double _yCenter, treeNode<BeachSection>* _section) {
        site = _site;
        x = _x;
        y = _y;
        yCenter = _yCenter;
        beachSection = _section;
    }
};
struct CircleEventQueue {
    treeNode<CircleEvent>* firstEvent;
    RBTree<CircleEvent> eventQueue;

    CircleEventQueue() : firstEvent(nullptr) {};
    ~CircleEventQueue() {};

    void addCircleEvent(treeNode<BeachSection>* section) {
        if (!section) return;
        treeNode<BeachSection>* lSection = section->prev;
        treeNode<BeachSection>* rSection = section->next;
        if (!lSection || !rSection) { return; }

        Point2 lSite = lSection->data.site->p;
        Point2 cSite = section->data.site->p;
        Point2 rSite = rSection->data.site->p;

        // If site of left beachsection is same as site of
        // right beachsection, there can't be convergence
        if (lSection == rSection) { return; }

        // Find the circumscribed circle for the three sites associated
        // with the beachsection triplet.
        // http://mathforum.org/library/drmath/view/55002.html
        // The bottom-most part of the circumcircle is our Fortune 'circle
        // event', and its center is a vertex potentially part of the final
        // Voronoi diagram.
        double bx = cSite.x;
        double by = cSite.y;
        double ax = lSite.x - bx;
        double ay = lSite.y - by;
        double cx = rSite.x - bx;
        double cy = rSite.y - by;

        // If points l->c->r are clockwise, then center beach section does not
        // collapse, hence it can't end up as a vertex (we reuse 'd' here, which
        // sign is reverse of the orientation, hence we reverse the test.
        // http://en.wikipedia.org/wiki/Curve_orientation#Orientation_of_a_simple_polygon
        double d = 2 * (ax*cy - ay*cx);
        if (d >= -2e-12) { return; } //handles finite precision error

        double ha = ax*ax + ay*ay;
        double hc = cx*cx + cy*cy;
        double x = (cy*ha - ay*hc) / d;
        double y = (ax*hc - cx*ha) / d;
        double ycenter = y + by;

        CircleEvent circleEvent(section->data.site, x + bx, ycenter + sqrt(x*x + y*y), ycenter, section);

        // find insertion point in RB-tree:
        // circle events are ordered from smallest to largest
        treeNode<CircleEvent>* predecessor = nullptr;
        treeNode<CircleEvent>* node = eventQueue.getRoot();
        while (node) {
            CircleEvent& nodeEvent = node->data;
            if (circleEvent.y < nodeEvent.y || (circleEvent.y == nodeEvent.y && circleEvent.x <= nodeEvent.x)) {
                if (node->left) {
                    node = node->left;
                }
                else {
                    predecessor = node->prev;
                    break;
                }
            }
            else {
                if (node->right) {
                    node = node->right;
                }
                else {
                    predecessor = node;
                    break;
                }
            }
        }
        treeNode<CircleEvent>* newEvent = eventQueue.insertSuccessor(predecessor, circleEvent);
        section->data.circleEvent = newEvent;
        if (!predecessor) {
            firstEvent = newEvent;
        }
    }
    void removeCircleEvent(treeNode<BeachSection>* section) {
        treeNode<CircleEvent>* circleEvent = section->data.circleEvent;
        if (circleEvent) {
            if (!circleEvent->prev) {
                firstEvent = circleEvent->next;
            }
            eventQueue.removeNode(circleEvent);
            section->data.circleEvent = nullptr;
        }
    }
};
struct cellBoundingBox {
    double xmin;
    double ymin;
    double width;
    double height;

    cellBoundingBox(double _xmin, double _ymin, double _xmax, double _ymax) :
        xmin(_xmin), ymin(_ymin), width(_xmax - _xmin), height(_ymax - _ymin) {};
};
struct Cell {
    Site site;
    std::vector<HalfEdge*> halfEdges;
    bool closeMe;

    Cell() : closeMe(false) {};
    Cell(Point2 _site) : site(_site, this), closeMe(false) {};

    std::vector<Cell*> getNeighbors() {
        std::vector<Cell*> neighbors;
        Edge* e;

        size_t edgeCount = halfEdges.size();
        while (edgeCount--) {
            e = halfEdges[edgeCount]->edge;
            if (e->lSite && e->lSite != &site) {
                neighbors.push_back(e->lSite->cell);
            }
            else if (e->rSite && e->rSite != &site) {
                neighbors.push_back(e->rSite->cell);
            }
        }

        return neighbors;
    }
    cellBoundingBox getBoundingBox() {
        size_t edgeCount = halfEdges.size();
        double xmin = std::numeric_limits<double>::infinity();
        double ymin = xmin;
        double xmax = -xmin;
        double ymax = xmax;

        Point2* vert;
        while (edgeCount--) {
            vert = halfEdges[edgeCount]->startPoint();

            double vx = vert->x;
            double vy = vert->y;

            if (vx < xmin) xmin = vx;
            if (vy < ymin) ymin = vy;
            if (vx > xmax) xmax = vx;
            if (vy > ymax) ymax = vy;
        }

        return cellBoundingBox(xmin, ymin, xmax, ymax);
    }
// Return whether a point is inside, on, or outside the cell:
//   -1: point is outside the perimeter of the cell
//    0: point is on the perimeter of the cell
//    1: point is inside the perimeter of the cell
    int pointIntersection(double x, double y) {
        // Check if point in polygon. Since all polygons of a Voronoi
        // diagram are convex, then:
        // http://paulbourke.net/geometry/polygonmesh/
        // Solution 3 (2D):
        //   "If the polygon is convex then one can consider the polygon
        //   "as a 'path' from the first vertex. A point is on the interior
        //   "of this polygons if it is always on the same side of all the
        //   "line segments making up the path. ...
        //   "(y - y0) (x1 - x0) - (x - x0) (y1 - y0)
        //   "if it is less than 0 then P is to the right of the line segment,
        //   "if greater than 0 it is to the left, if equal to 0 then it lies
        //   "on the line segment"
        HalfEdge* he;
        size_t edgeCount = halfEdges.size();
        Point2 p0;
        Point2 p1;
        double r;

        while (edgeCount--) {
            he = halfEdges[edgeCount];
            p0 = *he->startPoint();
            p1 = *he->endPoint();
            r = (y - p0.y)*(p1.x - p0.x) - (x - p0.x)*(p1.y - p0.y);

            if (r == 0) {
                return 0;
            }
            if (r > 0) {
                return -1;
            }
        }
        return 1;
    }
    static bool edgesCCW(HalfEdge* a, HalfEdge* b) {
        return a->angle > b->angle;
    }
};

class Diagram {
 public:
    std::vector<Cell*> cells;
    std::vector<Edge*> edges;
    std::vector<Point2*> vertices;

    ~Diagram() {
        for (Cell* c : cells) {
            freeVecMemory(c->halfEdges);
        }
    }
    void printDiagram() {
        if (cells.size()) {
            for (Cell* c : cells) {
                cout << c->site.p << endl;
                for (HalfEdge* e : c->halfEdges) {
                    cout << '\t' << *e->startPoint() << endl;
                }
                cout << endl;
            }
            for (Edge* e : edges) {
                cout << *e->vertA << " -> " << *e->vertB << endl;
            }
            cout << endl;
        }
        else {
            for (Cell* c : tmpCells) {
                cout << c->site.p << endl;
                for (HalfEdge* e : c->halfEdges) {
                    Point2* pS = e->startPoint();
                    Point2* pE = e->endPoint();

                    cout << '\t';
                    if (pS) cout << *pS;
                    else cout << "null";
                    cout << " -> ";
                    if (pE) cout << *pE;
                    else cout << "null";
                    cout << endl;
                }
                cout << endl;
            }
            for (Edge* e : tmpEdges) {
                if (e->vertA)
                    cout << *e->vertA;
                else
                    cout << "null";
                cout << " -> ";
                if (e->vertB)
                    cout << *e->vertB;
                else
                    cout << "null";
                cout << endl;
            }
            cout << endl;
        }
        cout << "=============================================" << endl;
    }
 private:
    friend class VoronoiDiagramGenerator;

    std::set<Cell*> tmpCells;
    std::set<Edge*> tmpEdges;
    std::set<Point2*> tmpVertices;

    Point2* createVertex(double x, double y) {
        Point2* vert = new Point2(x, y);
        tmpVertices.insert(vert);

        return vert;
    }
    Cell* createCell(Point2 site) {
        Cell* cell = new Cell(site);
        tmpCells.insert(cell);

        return cell;
    }
    Edge* createEdge(Site* lSite, Site* rSite, Point2* vertA, Point2* vertB) {
        Edge* edge = new Edge(lSite, rSite);
        tmpEdges.insert(edge);

        if (vertA) edge->setStartPoint(lSite, rSite, vertA);
        if (vertB) edge->setEndPoint(lSite, rSite, vertB);

        lSite->cell->halfEdges.push_back(new HalfEdge(edge, lSite, rSite));
        rSite->cell->halfEdges.push_back(new HalfEdge(edge, rSite, lSite));

        return edge;
    }
    Edge* createBorderEdge(Site* lSite, Point2* vertA, Point2* vertB) {
        Edge* edge = new Edge(lSite, nullptr, vertA, vertB);
        tmpEdges.insert(edge);

        return edge;
    }

// connect dangling edges (not if a cursory test tells us
// it is not going to be visible.
// return value:
//   false: the dangling endpoint couldn't be connected
//   true: the dangling endpoint could be connected
    bool connectEdge(Edge* edge, BoundingBox bbox) {
        // skip if end point already connected
        Point2* va = edge->vertA;
        Point2* vb = edge->vertB;
        if (vb) { return true; }

        // make local copy for speed
        Site* lSite = edge->lSite;
        Site* rSite = edge->rSite;
        double lx = lSite->p.x;
        double ly = lSite->p.y;
        double rx = rSite->p.x;
        double ry = rSite->p.y;
        double fx = (lx + rx) / 2.0;
        double fy = (ly + ry) / 2.0;
        double fm, fb;

        // if we reach here, this means cells which use this edge will need
        // to be closed, whether because the edge was removed, or because it
        // was connected to the bounding box.
        lSite->cell->closeMe = true;
        rSite->cell->closeMe = true;

        // get the line equation of the bisector if line is not vertical
        if (ry != ly) {
            fm = (lx - rx) / (ry - ly);
            fb = fy - fm*fx;
        }

        // remember, direction of line (relative to left site):
        // upward: left.x < right.x
        // downward: left.x > right.x
        // horizontal: left.x == right.x
        // upward: left.x < right.x
        // rightward: left.y < right.y
        // leftward: left.y > right.y
        // vertical: left.y == right.y

        // depending on the direction, find the best side of the
        // bounding box to use to determine a reasonable start point
        // While at it, since we have the values which define the line,
        // clip the end of va if it is outside the bbox.

        // special case: vertical line
        if (ry == ly) {
            // doesn't intersect with viewport
            if (fx < bbox.xL || fx >= bbox.xR) { return false; }
            // downward
            if (lx > rx) {
                if (!va || va->y < bbox.yT) {
                    va = createVertex(fx, bbox.yT);
                }
                else if (va->y >= bbox.yB) {
                    return false;
                }
                vb = createVertex(fx, bbox.yB);
            }
                // upward
            else {
                if (!va || va->y > bbox.yB) {
                    va = createVertex(fx, bbox.yB);
                }
                else if (va->y < bbox.yT) {
                    return false;
                }
                vb = createVertex(fx, bbox.yT);
            }
        }
            // closer to vertical than horizontal, connect start point to the
            // top or bottom side of the bounding box
        else if (fm < -1.0 || fm > 1.0) {
            // downward
            if (lx > rx) {
                if (!va || va->y < bbox.yT) {
                    va = createVertex((bbox.yT - fb) / fm, bbox.yT);
                }
                else if (va->y >= bbox.yB) {
                    return false;
                }
                vb = createVertex((bbox.yB - fb) / fm, bbox.yB);
            }
                // upward
            else {
                if (!va || va->y > bbox.yB) {
                    va = createVertex((bbox.yB - fb) / fm, bbox.yB);
                }
                else if (va->y < bbox.yT) {
                    return false;
                }
                vb = createVertex((bbox.yT - fb) / fm, bbox.yT);
            }
        }
            // closer to horizontal than vertical, connect start point to the
            // left or right side of the bounding box
        else {
            // rightward
            if (ly < ry) {
                if (!va || va->x < bbox.xL) {
                    va = createVertex(bbox.xL, fm*bbox.xL + fb);
                }
                else if (va->x >= bbox.xR) {
                    return false;
                }
                vb = createVertex(bbox.xR, fm*bbox.xR + fb);
            }
                // leftward
            else {
                if (!va || va->x > bbox.xR) {
                    va = createVertex(bbox.xR, fm*bbox.xR + fb);
                }
                else if (va->x < bbox.xL) {
                    return false;
                }
                vb = createVertex(bbox.xL, fm*bbox.xL + fb);
            }
        }
        edge->vertA = va;
        edge->vertB = vb;

        return true;
    }

// line-clipping code taken from:
//   Liang-Barsky function by Daniel White
//   http://www.skytopia.com/project/articles/compsci/clipping.html
// A bit modified to minimize code paths
    bool clipEdge(Edge* edge, BoundingBox bbox) {
        double ax = edge->vertA->x;
        double ay = edge->vertA->y;
        double bx = edge->vertB->x;
        double by = edge->vertB->y;
        double t0 = 0;
        double t1 = 1;
        double dx = bx - ax;
        double dy = by - ay;
        // left
        double q = ax - bbox.xL;
        if (dx == 0 && q<0) { return false; }
        double r = -q / dx;
        if (dx < 0) {
            if (r < t0) { return false; }
            if (r < t1) { t1 = r; }
        }
        else if (dx>0) {
            if (r > t1) { return false; }
            if (r > t0) { t0 = r; }
        }
        // right
        q = bbox.xR - ax;
        if (dx == 0 && q<0) { return false; }
        r = q / dx;
        if (dx<0) {
            if (r>t1) { return false; }
            if (r>t0) { t0 = r; }
        }
        else if (dx>0) {
            if (r<t0) { return false; }
            if (r<t1) { t1 = r; }
        }
        // top
        q = ay - bbox.yT;
        if (dy == 0 && q<0) { return false; }
        r = -q / dy;
        if (dy<0) {
            if (r<t0) { return false; }
            if (r<t1) { t1 = r; }
        }
        else if (dy>0) {
            if (r>t1) { return false; }
            if (r>t0) { t0 = r; }
        }
        // bottom
        q = bbox.yB - ay;
        if (dy == 0 && q<0) { return false; }
        r = q / dy;
        if (dy<0) {
            if (r>t1) { return false; }
            if (r>t0) { t0 = r; }
        }
        else if (dy>0) {
            if (r<t0) { return false; }
            if (r<t1) { t1 = r; }
        }

        // if we reach this point, Voronoi edge is within bbox

        // if t0 > 0, va needs to change
        // we need to create a new vertex rather
        // than modifying the existing one, since the existing
        // one is likely shared with at least another edge
        if (t0 > 0) {
            edge->vertA = createVertex(ax + t0*dx, ay + t0*dy);
        }

        // if t1 < 1, vb needs to change
        // we need to create a new vertex rather
        // than modifying the existing one, since the existing
        // one is likely shared with at least another edge
        if (t1 < 1) {
            edge->vertB = createVertex(ax + t1*dx, ay + t1*dy);
        }

        // va and/or vb were clipped, thus we will need to close
        // cells which use this edge.
        if (t0 > 0 || t1 < 1) {
            edge->lSite->cell->closeMe = true;
            edge->rSite->cell->closeMe = true;
        }

        return true;
    }

// Connect/cut edges at bounding box
    void clipEdges(BoundingBox bbox) {
        // connect all dangling edges to bounding box
        // or get rid of them if it can't be done
        std::vector<Edge*> toRemove;

        for(Edge* edge : tmpEdges) {
            // edge is removed if:
            //   it is wholly outside the bounding box
            //   it is looking more like a point than a line
            if (!connectEdge(edge, bbox) || !clipEdge(edge, bbox)
                || (eq_withEpsilon(edge->vertA->x, edge->vertB->x) && eq_withEpsilon(edge->vertA->y, edge->vertB->y))) {
                edge->vertA = edge->vertB = nullptr;
                toRemove.push_back(edge);
            }
        }
        for (Edge* e : toRemove) {
            std::vector<HalfEdge*>* halfEdges;
            size_t edgeCount;
            HalfEdge* he;

            //remove lSite halfEdges
            halfEdges = &e->lSite->cell->halfEdges;
            edgeCount = halfEdges->size();
            while (edgeCount) {
                he = halfEdges->at(--edgeCount);
                if (he->edge == e) {
                    halfEdges->erase(halfEdges->begin() + edgeCount);
                    delete he;
                }
            }

            //remove rSite halfEdges
            halfEdges = &e->rSite->cell->halfEdges;
            edgeCount = halfEdges->size();
            while (edgeCount) {
                he = halfEdges->at(--edgeCount);
                if (he->edge == e) {
                    halfEdges->erase(halfEdges->begin() + edgeCount);
                    delete he;
                }
            }

            //remove edge
            tmpEdges.erase(e);
            delete e;
        }
    }

// Close the cells.
// The cells are bound by the supplied bounding box.
// Each cell refers to its associated site, and a list
// of halfedges ordered counterclockwise.
    void closeCells(BoundingBox bbox) {
        Point2* va;
        Point2* vb;
        Point2* vz;
        Edge* edge;
        std::vector<HalfEdge*>* halfEdges;

        for (Cell* cell : tmpCells) {
            // prune, order halfedges counterclockwise, then add missing ones
            // required to close cells
            halfEdges = &cell->halfEdges;
            std::sort(halfEdges->begin(), halfEdges->end(), Cell::edgesCCW);
            size_t nHalfEdges = halfEdges->size();
            if (!nHalfEdges) {
                continue;
            }
            if (!cell->closeMe) {
                continue;
            }
            // find first 'unclosed' point.
            // an 'unclosed' point will be the end point of a halfedge which
            // does not match the start point of the following halfedge
            size_t iLeft = 0;
            while (iLeft < nHalfEdges) {
                va = (*halfEdges)[iLeft]->endPoint();
                vz = (*halfEdges)[(iLeft + 1) % nHalfEdges]->startPoint();
                // if end point is not equal to start point, we need to add the missing
                // halfedge(s) up to vz
                if (abs(va->x - vz->x) >= EPSILON || abs(va->y - vz->y) >= EPSILON) {
                    // find entry point:
                    bool foundEntryPoint = false;
                    bool finished = false;
                    bool lastBorderSegment = false;

                    // walk downward along left side
                    if (eq_withEpsilon(va->x, bbox.xL) && lt_withEpsilon(va->y, bbox.yB)) {
                        foundEntryPoint = true;
                        lastBorderSegment = eq_withEpsilon(vz->x, bbox.xL);
                        vb = lastBorderSegment ? vz : createVertex(bbox.xL, lastBorderSegment ? vz->y : bbox.yB);
                        edge = createBorderEdge(&cell->site, va, vb);
                        ++iLeft;
                        halfEdges->insert(halfEdges->begin() + iLeft, new HalfEdge(edge, &cell->site, nullptr));
                        ++nHalfEdges;
                        if (lastBorderSegment) { finished = true; }
                        va = vb;
                    }

                    // walk rightward along bottom side
                    if ((!finished && foundEntryPoint) || (eq_withEpsilon(va->y, bbox.yB) && lt_withEpsilon(va->x, bbox.xR))) {
                        foundEntryPoint = true;
                        lastBorderSegment = eq_withEpsilon(vz->y, bbox.yB);
                        vb = lastBorderSegment ? vz : createVertex(lastBorderSegment ? vz->x : bbox.xR, bbox.yB);
                        edge = createBorderEdge(&cell->site, va, vb);
                        ++iLeft;
                        halfEdges->insert(halfEdges->begin() + iLeft, new HalfEdge(edge, &cell->site, nullptr));
                        ++nHalfEdges;
                        if (lastBorderSegment) { finished = true; }
                        va = vb;
                    }

                    // walk upward along right side
                    if ((!finished && foundEntryPoint) || (eq_withEpsilon(va->x, bbox.xR) && gt_withEpsilon(va->y, bbox.yT))) {
                        foundEntryPoint = true;
                        lastBorderSegment = eq_withEpsilon(vz->x, bbox.xR);
                        vb = lastBorderSegment ? vz : createVertex(bbox.xR, lastBorderSegment ? vz->y : bbox.yT);
                        edge = createBorderEdge(&cell->site, va, vb);
                        ++iLeft;
                        halfEdges->insert(halfEdges->begin() + iLeft, new HalfEdge(edge, &cell->site, nullptr));
                        ++nHalfEdges;
                        if (lastBorderSegment) { finished = true; }
                        va = vb;
                    }

                    // walk leftward along top side
                    if ((!finished && foundEntryPoint) || (eq_withEpsilon(va->y, bbox.yT) && gt_withEpsilon(va->x, bbox.xL))) {
                        lastBorderSegment = eq_withEpsilon(vz->y, bbox.yT);
                        vb = lastBorderSegment ? vz : createVertex(lastBorderSegment ? vz->x : bbox.xL, bbox.yT);
                        edge = createBorderEdge(&cell->site, va, vb);
                        ++iLeft;
                        halfEdges->insert(halfEdges->begin() + iLeft, new HalfEdge(edge, &cell->site, nullptr));
                        ++nHalfEdges;
                        if (lastBorderSegment) { finished = true; }
                        va = vb;

                        // walk downward along left side
                        if (!finished) {
                            lastBorderSegment = eq_withEpsilon(vz->x, bbox.xL);
                            vb = lastBorderSegment ? vz : createVertex(bbox.xL, lastBorderSegment ? vz->y : bbox.yB);
                            edge = createBorderEdge(&cell->site, va, vb);
                            ++iLeft;
                            halfEdges->insert(halfEdges->begin() + iLeft, new HalfEdge(edge, &cell->site, nullptr));
                            ++nHalfEdges;
                            if (lastBorderSegment) { finished = true; }
                            va = vb;
                        }

                        // walk rightward along bottom side
                        if (!finished) {
                            lastBorderSegment = eq_withEpsilon(vz->y, bbox.yB);
                            vb = lastBorderSegment ? vz : createVertex(lastBorderSegment ? vz->x : bbox.xR, bbox.yB);
                            edge = createBorderEdge(&cell->site, va, vb);
                            ++iLeft;
                            halfEdges->insert(halfEdges->begin() + iLeft, new HalfEdge(edge, &cell->site, nullptr));
                            ++nHalfEdges;
                            if (lastBorderSegment) { finished = true; }
                            va = vb;
                        }

                        // walk upward along right side
                        if (!finished) {
                            lastBorderSegment = eq_withEpsilon(vz->x, bbox.xR);
                            vb = lastBorderSegment ? vz : createVertex(bbox.xR, lastBorderSegment ? vz->y : bbox.yT);
                            edge = createBorderEdge(&cell->site, va, vb);
                            ++iLeft;
                            halfEdges->insert(halfEdges->begin() + iLeft, new HalfEdge(edge, &cell->site, nullptr));
                            ++nHalfEdges;
                        }
                    }
                }
                ++iLeft;
            }
            cell->closeMe = false;
        }
    }
    void finalize() {
        cells.reserve(tmpCells.size());
        for (Cell* c : tmpCells) {
            cells.push_back(c);
        }
        tmpCells.clear();

        edges.reserve(tmpEdges.size());
        for (Edge* e : tmpEdges) {
            edges.push_back(e);
        }
        tmpEdges.clear();

        vertices.reserve(tmpVertices.size());
        for (Point2* v : tmpVertices) {
            vertices.push_back(v);
        }
        tmpVertices.clear();
    }

    template <typename T>
    void freeVecMemory(T& t) {
        T tmp;
        t.swap(tmp);
    }
};


bool pointComparator(Point2* a, Point2* b) {
    double r = b->y - a->y;
    if (r < 0) return true;
    else if (r == 0) {
        if (b->x - a->x < 0) return true;
        else return false;
    }
    else return false;
}

bool halfEdgesCW(HalfEdge* e1, HalfEdge* e2) {
    return e1->angle < e2->angle;
}


class VoronoiDiagramGenerator {
 public:
    VoronoiDiagramGenerator() : circleEventQueue(nullptr), siteEventQueue(nullptr), beachLine(nullptr) {};
    ~VoronoiDiagramGenerator() {};

    Diagram* compute(std::vector<Point2>& sites, BoundingBox bbox) {
        siteEventQueue = new std::vector<Point2*>();
        boundingBox = bbox;

        for (size_t i = 0; i < sites.size(); ++i) {
            //sanitize sites by quantizing to integer multiple of epsilon
            sites[i].x = round(sites[i].x / EPSILON)*EPSILON;
            sites[i].y = round(sites[i].y / EPSILON)*EPSILON;

            siteEventQueue->push_back(&(sites[i]));
        }

        diagram = new Diagram();
        circleEventQueue = new CircleEventQueue();
        beachLine = new RBTree<BeachSection>();

        // Initialize site event queue
        std::sort(siteEventQueue->begin(), siteEventQueue->end(), pointComparator);

        // process queue
        Point2* site = siteEventQueue->empty() ? nullptr : siteEventQueue->back();
        if (!siteEventQueue->empty()) siteEventQueue->pop_back();
        treeNode<CircleEvent>* circle;

        // main loop
        for (;;) {
            // figure out whether to handle a site or circle event
            // for this we find out if there is a site event and if it is
            // 'earlier' than the circle event
            circle = circleEventQueue->firstEvent;

            // add beach section
            if (site && (!circle || site->y < circle->data.y || (site->y == circle->data.y && site->x < circle->data.x))) {
                // first create cell for new site
                Cell* cell = diagram->createCell(*site);
                // then create a beachsection for that site
                addBeachSection(&cell->site);

                site = siteEventQueue->empty() ? nullptr : siteEventQueue->back();
                if (!siteEventQueue->empty()) siteEventQueue->pop_back();
            }

                // remove beach section
            else if (circle)
                removeBeachSection(circle->data.beachSection);

                // all done, quit
            else
                break;
        }

        // wrapping-up:
        //   connect dangling edges to bounding box
        //   cut edges as per bounding box
        //   discard edges completely outside bounding box
        //   discard edges which are point-like
        diagram->clipEdges(boundingBox);

        //   add missing edges in order to close open cells
        diagram->closeCells(boundingBox);

        diagram->finalize();

        delete circleEventQueue;
        circleEventQueue = nullptr;

        delete siteEventQueue;
        siteEventQueue = nullptr;

        delete beachLine;
        beachLine = nullptr;

        return diagram;
    }
    Diagram* relax() {
        std::vector<Point2> sites;
        std::vector<Point2> verts;
        std::vector<Vector2> vectors;
        //replace each site with its cell's centroid:
        //    subdivide the cell into adjacent triangles
        //    find those triangles' centroids (by averaging corners)
        //    and areas (by computing vector cross product magnitude)
        //    combine the triangles' centroids through weighted average
        //	  to get the whole cell's centroid
        for (Cell* c : diagram->cells) {
            size_t edgeCount = c->halfEdges.size();
            verts.resize(edgeCount);
            vectors.resize(edgeCount);

            for (size_t i = 0; i < edgeCount; ++i) {
                verts[i] = *c->halfEdges[i]->startPoint();
                vectors[i] = *c->halfEdges[i]->startPoint() - verts[0];
            }

            Point2 centroid(0.0, 0.0);
            double totalArea = 0.0;
            for (size_t i = 1; i < edgeCount-1; ++i) {
                double area = (vectors[i+1].x*vectors[i].y - vectors[i+1].y*vectors[i].x)/2;
                totalArea += area;
                centroid.x += area*(verts[0].x + verts[i].x + verts[i + 1].x) / 3;
                centroid.y += area*(verts[0].y + verts[i].y + verts[i + 1].y) / 3;
            }
            centroid.x /= totalArea;
            centroid.y /= totalArea;
            sites.push_back(centroid);
        }

        //then recompute the diagram using the cells' centroids
        compute(sites, boundingBox);

        return diagram;
    }
 private:
    Diagram* diagram;
    CircleEventQueue* circleEventQueue;
    std::vector<Point2*>* siteEventQueue;
    BoundingBox	boundingBox;

    void printBeachLine() {
        treeNode<BeachSection>* section = beachLine->getFirst(beachLine->getRoot());

        while (section) {
            cout << section->data.site->p << endl;
            section = section->next;
        }
        if(section) cout << section->data.site->p << endl;
        cout << endl << endl;
    }

    //BeachLine
    RBTree<BeachSection>* beachLine;
    inline void detachBeachSection(treeNode<BeachSection>* section) {
        circleEventQueue->removeCircleEvent(section);
        beachLine->removeNode(section);
    }
    treeNode<BeachSection>* addBeachSection(Site* site) {
        double x = site->p.x;
        double directrix = site->p.y;

        // find the left and right beach sections which will surround the newly
        // created beach section.
        treeNode<BeachSection>* lSection = nullptr;
        treeNode<BeachSection>* rSection = nullptr;
        treeNode<BeachSection>* node = beachLine->getRoot();

        double dxl, dxr;
        while (node) {
            dxl = leftBreakpoint(node, directrix) - x;
            if (dxl > EPSILON) {
                //falls somewhere before the left edge of the beachsection
                node = node->left;
            }
            else {
                dxr = x - rightBreakpoint(node, directrix);
                if (dxr > EPSILON) {
                    //falls somewhere after the right edge of the beachsection
                    if (!node->right) {
                        lSection = node;
                        break;
                    }
                    node = node->right;
                }
                else {
                    if (dxl > -EPSILON) {
                        //falls exactly on the left edge of the beachsection
                        lSection = node->prev;
                        rSection = node;
                    }
                    else if (dxr > -EPSILON) {
                        //exactly on the right edge of the beachsection
                        lSection = node;
                        rSection = node->next;
                    }
                    else {
                        // falls somewhere in the middle of the beachsection
                        lSection = rSection = node;
                    }
                    break;
                }
            }
        }
        // at this point, lSection and/or rSection could be null.

        // add a new beach section for the site to the RB-tree
        BeachSection section(site);
        treeNode<BeachSection>* newSection = beachLine->insertSuccessor(lSection, section);

        // cases:

        // [lSection,rSection] where lSection == rSection
        // most likely case: new beach section split an existing beach
        // section.
        // This case means:
        //   one new transition appears
        //   the left and right beach section might be collapsing as a result
        //   two new nodes added to the RB-tree
        if (lSection && lSection == rSection) {
            // invalidate circle event of split beach section
            circleEventQueue->removeCircleEvent(lSection);

            // split the beach section into two separate beach sections
            BeachSection copy = BeachSection(lSection->data.site);
            rSection = beachLine->insertSuccessor(newSection, copy);

            // since we have a new transition between two beach sections, a new edge is born
            Edge* newEdge = diagram->createEdge(lSection->data.site, newSection->data.site, nullptr, nullptr);
            newSection->data.edge = rSection->data.edge = newEdge;

            // check whether the left and right beach sections are collapsing
            // and if so create circle events
            circleEventQueue->addCircleEvent(lSection);
            circleEventQueue->addCircleEvent(rSection);

            return newSection;
        }

        // [lSection,rSection] where lSection != rSection
        // fairly unlikely case: new beach section falls *exactly* in between two
        // existing beach sections
        // This case means:
        //   one transition disappears
        //   two new transitions appear
        //   the left and right beach section might be collapsing as a result
        //   only one new node added to the RB-tree
        if (lSection && rSection && lSection != rSection) {
            // invalidate circle events of left and right sites
            circleEventQueue->removeCircleEvent(lSection);
            circleEventQueue->removeCircleEvent(rSection);

            // an existing transition disappears, meaning a vertex is defined at
            // the disappearance point.
            // since the disappearance is caused by the new beachsection, the
            // vertex is at the center of the circumscribed circle of the left,
            // new and right beachsections.
            // http://mathforum.org/library/drmath/view/55002.html
            Site* lSite = lSection->data.site;
            Site* rSite = rSection->data.site;
            Point2& lP = lSite->p;
            Point2& sP = site->p;
            Point2& rP = rSite->p;
            double ax = lP.x;
            double ay = lP.y;
            double bx = sP.x - ax;
            double by = sP.y - ay;
            double cx = rP.x - ax;
            double cy = rP.y - ay;
            double d = 2 * (bx*cy - by*cx);
            double hb = bx*bx + by*by;
            double hc = cx*cx + cy*cy;
            Point2* vertex = diagram->createVertex((cy*hb - by*hc) / d + ax, (bx*hc - cx*hb) / d + ay);

            // one transition disappears
            rSection->data.edge->setStartPoint(lSite, rSite, vertex);

            // two new transitions appear at the new vertex location
            newSection->data.edge = diagram->createEdge(lSite, site, nullptr, vertex);
            rSection->data.edge = diagram->createEdge(site, rSite, nullptr, vertex);

            // check whether the left and right beach sections are collapsing
            // and if so create circle events
            circleEventQueue->addCircleEvent(lSection);
            circleEventQueue->addCircleEvent(rSection);

            return newSection;
        }

        // [lSection,null]
        // very unlikely case: new beach section is the *last* beach section
        // on the beachline -- this can happen only if all the previous beach
        // sections currently on the beachline share the same y value as
        // the new beach section.
        // This case means:
        //   one new transition appears
        //   no collapsing beach section as a result
        //   new beach section becomes right-most node of the RB-tree
        if (lSection && !rSection) {
            newSection->data.edge = diagram->createEdge(lSection->data.site, newSection->data.site, nullptr, nullptr);
            return newSection;
        }

        // [null,null]
        // least likely case: new beach section is the first beach section on the
        // beachline.
        // This case means:
        //   no new transition appears
        //   no collapsing beach section
        //   new beachsection becomes root of the RB-tree
        if (!lSection && !rSection) {
            return newSection;
        }

        return nullptr;
    }
    void removeBeachSection(treeNode<BeachSection>* section) {
        CircleEvent circle = section->data.circleEvent->data;
        double x = circle.x;
        double y = circle.yCenter;
        Point2* vertex = diagram->createVertex(x, y);
        treeNode<BeachSection>* prev = section->prev;
        treeNode<BeachSection>* next = section->next;
        std::vector<treeNode<BeachSection>*> disappearingTransitions;
        std::forward_list<treeNode<BeachSection>*> toBeDetached;
        disappearingTransitions.push_back(section);

        // save collapsed beachsection to be detached from beachline
        toBeDetached.push_front(section);

        // there could be more than one empty arc at the deletion point, this
        // happens when more than two edges are linked by the same vertex,
        // so we will collect all those edges by looking along both sides of
        // the deletion point.

        // look left
        treeNode<BeachSection>* lSection = prev;
        while (lSection->data.circleEvent
            && eq_withEpsilon(x, lSection->data.circleEvent->data.x)
            && eq_withEpsilon(y, lSection->data.circleEvent->data.yCenter)) {
            prev = lSection->prev;
            disappearingTransitions.insert(disappearingTransitions.begin(), lSection);
            toBeDetached.push_front(lSection);
            lSection = prev;
        }
        // even though it is not disappearing, I will also add the beach section
        // immediately to the left of the left-most collapsed beach section, for
        // convenience, since we need to refer to it later as this beach section
        // is the 'left' site of an edge for which a start point is set.
        disappearingTransitions.insert(disappearingTransitions.begin(), lSection);
        circleEventQueue->removeCircleEvent(lSection);

        // look right
        treeNode<BeachSection>* rSection = next;
        while (rSection->data.circleEvent
            && eq_withEpsilon(x, rSection->data.circleEvent->data.x)
            && eq_withEpsilon(y, rSection->data.circleEvent->data.yCenter)) {
            next = rSection->next;
            disappearingTransitions.push_back(rSection);
            toBeDetached.push_front(rSection);
            rSection = next;
        }
        // we also have to add the beach section immediately to the right of the
        // right-most collapsed beach section, since there is also a disappearing
        // transition representing an edge's start point on its left.
        disappearingTransitions.push_back(rSection);
        circleEventQueue->removeCircleEvent(rSection);

        // walk through all the disappearing transitions between beach sections and
        // set the start point of their (implied) edges.
        size_t nSections = disappearingTransitions.size();
        for (size_t i = 1; i < nSections; ++i) {
            rSection = disappearingTransitions[i];
            lSection = disappearingTransitions[i-1];
            rSection->data.edge->setStartPoint(lSection->data.site, rSection->data.site, vertex);
        }

        // create a new edge as we have now a new transition between
        // two beach sections which were previously not adjacent.
        // since this edge appears as a new vertex is defined, the vertex
        // actually defines an end point of the edge (relative to the site
        // on the left)
        lSection = disappearingTransitions[0];
        rSection = disappearingTransitions[nSections - 1];
        rSection->data.edge = diagram->createEdge(lSection->data.site, rSection->data.site, nullptr, vertex);

        for (treeNode<BeachSection>* section : toBeDetached) {
            detachBeachSection(section);
        }

        // create circle events if any for beach sections left in the beachline
        // adjacent to collapsed sections
        circleEventQueue->addCircleEvent(lSection);
        circleEventQueue->addCircleEvent(rSection);
    }

// calculate the left break point of a particular beach section,
// given a particular sweep line height
    double leftBreakpoint(treeNode<BeachSection>* section, double directrix) {
        Point2 site = section->data.site->p;
        double rfocx = site.x;
        double rfocy = site.y;
        double pby2 = rfocy - directrix;
        if (pby2 == 0) {
            // parabola in degenerate case where focus is on directrix
            return rfocx;
        }

        treeNode<BeachSection>* lSection = section->prev;
        if (!lSection) {
            return -std::numeric_limits<double>::infinity();
        }

        site = lSection->data.site->p;
        double lfocx = site.x;
        double lfocy = site.y;
        double plby2 = lfocy - directrix;
        if (plby2 == 0) {
            // parabola in degenerate case where focus is on directrix
            return lfocx;
        }

        double hl = lfocx - rfocx;
        double aby2 = (1 / pby2) - (1 / plby2);
        double b = hl / plby2;
        if (aby2 != 0) {
            return (-b + sqrt(b*b - 2 * aby2*(hl*hl / (-2 * plby2) - lfocy + plby2 / 2 + rfocy - pby2 / 2))) / aby2 + rfocx;
        }

        // if we get here, the two parabolas have the same
        // distance to the directrix, so the break point is midway
        return (rfocx + lfocx) / 2;
    }

// calculate the right break point of a particular beach section,
// given a particular sweep line height
    double rightBreakpoint(treeNode<BeachSection>* section, double directrix) {
        treeNode<BeachSection>* rSection = section->next;
        if (rSection) {
            return leftBreakpoint(rSection, directrix);
        }

        Point2 site = section->data.site->p;
        if (site.y == directrix) {
            return site.x;
        }
        else {
            return std::numeric_limits<double>::infinity();
        }
    }
};
