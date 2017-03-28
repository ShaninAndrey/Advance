#include <iostream>
#include <vector>

struct segment_tree {
    private:
        struct node {
            int count_distinct;
            node *left_son, *right_son;

            explicit node(const int value) {
                count_distinct = value;
                left_son = right_son = NULL;
            }
            node(node *, node *);
        };

        int elements_count;
        node *root;

        // return segment right_segment_borderee based subarray
        node* build(const std::vector<int> &elements,
                    const int left_segment_border,
                    const int right_segment_border) const {

            if (left_segment_border == right_segment_border) {
                return new node(elements[left_segment_border]);
            }
            const int mid = (left_segment_border + right_segment_border) / 2;
            node *left_son = build(elements, left_segment_border, mid);
            node *right_son = build(elements, mid + 1, right_segment_border);
            return new node(left_son, right_son);
        }

        // make new root
        node* update(const node *current_node,
                     const int left_segment_border,
                     const int right_segment_border,
                     const int update_position,
                     const int new_value) {

            if (left_segment_border == right_segment_border &&
                left_segment_border == update_position) {
                return new node(new_value);
            }
            const int mid = (left_segment_border + right_segment_border) / 2;
            if (mid >= update_position) {
                node *left_son = update(current_node->left_son,
                                        left_segment_border,
                                        mid,
                                        update_position,
                                        new_value);

                return new node(left_son, current_node->right_son);
            } else {
                node *right_son = update(current_node->right_son,
                                         mid + 1,
                                         right_segment_border,
                                         update_position,
                                         new_value);

                return new node(current_node->left_son, right_son);
            }
        }

    public:
        node* get_root() {
            return root;
        }
        segment_tree() { }
        explicit segment_tree(const std::vector<int> &elements) {
            elements_count = elements.size();
            root = build(elements, 0, elements_count - 1);
        }
        segment_tree(node *new_root, const int other_elements_count) {
            root = new_root;
            elements_count = other_elements_count;
        }
        segment_tree* update(const int update_position, const int new_value) {
            node *next = update(root, 0, elements_count - 1, update_position, new_value);
            return new segment_tree(next, elements_count);
        }

        // get answer of user query
        int get_right_border(const node*, const int, const int, const int);
};

struct persistant_segment_tree : segment_tree {
    private:
        std::vector<segment_tree*> versions;
        int max_value;

        void get_array_params(const std::vector<int> &elements,
                               std::vector<int> &ptr_to_next_same_element,
                               std::vector<int> &first_array_version) {

            std::vector<int> ptr_to_last_element(max_value, -1);
            for (int i = 0; i < elements.size(); ++i) {
                if (ptr_to_last_element[elements[i]] != -1) {
                    ptr_to_next_same_element[ptr_to_last_element[elements[i]]] = i;
                } else {
                    first_array_version[i] = 1;
                }
                ptr_to_last_element[elements[i]] = i;
            }
            for (int i = 0; i < max_value; ++i) {
                if (ptr_to_last_element[i] != -1) {
                    ptr_to_next_same_element[ptr_to_last_element[i]] = elements.size();
                }
            }
        }

        void build(const std::vector<int> &ptr_to_next_same_element) {
            segment_tree *t = versions[0];
            for (int i = 1; i < ptr_to_next_same_element.size(); ++i) {
                t = t->update(i - 1, 0);
                if (ptr_to_next_same_element[i - 1] < ptr_to_next_same_element.size()) {
                    t = t->update(ptr_to_next_same_element[i - 1], 1);
                }
                versions.push_back(t);
            }
        }

    public:
        persistant_segment_tree(const std::vector<int> &elements, const int &set_max_value) {
            max_value = set_max_value;

            std::vector<int> ptr_to_next_same_element(elements.size());
            std::vector<int> first_array_version(elements.size());
            get_array_params(elements, ptr_to_next_same_element, first_array_version);

            segment_tree *first_version = new segment_tree(first_array_version);
            versions.push_back(first_version);
            build(ptr_to_next_same_element);
        }

        // return segment tree which the user needs
        segment_tree* get_right_segment_tree(const int) const;
};

// Struct of user query
struct query {
    private:
        int first_val, second_val;

    public:
        query(const int Fval, const int Sval) {
            first_val = Fval;
            second_val = Sval;
        }
        int get_first_val() const {
            return first_val;
        }
        int get_second_val() const {
            return second_val;
        }
};

segment_tree::node::node(node *left, node *right) {
    left_son = left;
    right_son = right;
    count_distinct = left->count_distinct + right->count_distinct;
}

int segment_tree::get_right_border(const node *current_node,
                                   const int left_segment_border,
                                   const int right_segment_border,
                                   const int count_distinct) {

    if (left_segment_border == right_segment_border) {
        if (count_distinct == current_node->count_distinct) {
            return left_segment_border;
        }
        return -1;
    }

    const int middle = (left_segment_border + right_segment_border) / 2;
    if (current_node->left_son->count_distinct >= count_distinct) {
        return get_right_border(current_node->left_son, left_segment_border, middle, count_distinct);
    } else {
        return get_right_border(current_node->right_son,
                          middle + 1,
                          right_segment_border,
                          count_distinct - current_node->left_son->count_distinct);
    }
}

segment_tree* persistant_segment_tree::get_right_segment_tree(const int left_border) const {
    return versions[left_border];
}

// Read used data
void read(std::istream &input,
          int &array_length,
          int &max_value,
          int &query_count,
          std::vector<int> &elements,
          std::vector<query> &queries) {

    input >> array_length >> max_value;
    max_value++;
    elements.resize(array_length);
    for (int i = 0; i < array_length; ++i) {
        input >> elements[i];
    }
    input >> query_count;
    for (int i = 0; i < query_count; ++i) {
        int first_val, second_val;
        input >> first_val >> second_val;
        query current = query(first_val, second_val);
        queries.push_back(current);
    }
}

// Solve current problem
std::vector<int> solve(const std::vector<int> &elements,
           const std::vector<query> &queries,
           const int &max_value) {

    std::vector<int> answer(queries.size());
    persistant_segment_tree PT(elements, max_value);
    int last_answer = 0;
    for (int i = 0; i < queries.size(); ++i) {
        const int left_border = (queries[i].get_first_val() + last_answer) % elements.size();
        const int cnt_distinct = (queries[i].get_second_val() + last_answer) % (max_value - 1) + 1;

        segment_tree *current = PT.get_right_segment_tree(left_border);
        last_answer = current->get_right_border(current->get_root(),
                                                0,
                                                elements.size() - 1,
                                                cnt_distinct) + 1;
        answer[i] = last_answer;
    }
    return answer;
}

// Write output to user
void write(std::ostream &output,
           const std::vector<int> &answer) {

    for (int i = 0; i < answer.size(); ++i) {
        output << answer[i] << "\n";
    }
}

int main() {
    int array_length, max_value, query_count;
    std::vector<query> queries;
    std::vector<int> elements;
    std::vector<int> answer;
    read(std::cin,
         array_length,
         max_value,
         query_count,
         elements,
         queries);

    answer = solve(elements, queries, max_value);

    write(std::cout, answer);
}
