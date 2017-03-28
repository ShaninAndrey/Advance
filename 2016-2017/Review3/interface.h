#include <iostream>
#include <vector>

/*
* Struct to keeps user query
*/
struct query {
    private:

        int start_position, finish_position;

    public:

        query(const int, const int);

        /*
        * Get start_position value;
        */
        int get_start() const;

        /*
        * Get finish_position value;
        */
        int get_finish() const;
};

/*
* Struct of randomize treap of implicit key
*/
struct treap {
    private:

        int value, size, priority;

    public:

        treap *left, *right;

        /*
        * Get sz value;
        */
        int get_size() const;

        /*
        * Get prior value;
        */
        int get_priority() const;

        /*
        * Create node of treap with a predetermined value
        */
        explicit treap(const int);

        /*
        * return string with current subarray
        */
        std::vector<int> to_array() const;

        /*
        * Recount size of tree
        */
        void update();

        /*
        * Cut current treap to other two by predetermined key
        */
        void split(const int, treap * &, treap * &);
};

/*
* return size of treap
*/
int get_treap_size(const treap *);

/*
* return the union of treaps
*/
treap* merge(treap  *, treap *);

/*
* move predetermined segment to head
*/
void move(treap * &, const int, const int);

/*
* Read input data
*/
void read(std::istream &, int &, std::vector<query> &);

/*
* Do queries and set answer
*/
const std::vector<int> solve(const int &, const std::vector<query> &);

/*
* Write output data
*/
void write(std::ostream &, const std::vector<int> &);
