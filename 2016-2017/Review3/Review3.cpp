#include <iostream>
#include <sstream>
#include <vector>
#include <random>
#include "interface.h"

query::query(const int start, const int finish) {
    start_position = start;
    finish_position = finish;
}

int query::get_start() const {
    return start_position;
}

int query::get_finish() const {
    return finish_position;
}

treap::treap(const int val) {
    size = 1;
    value = val;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100000000);
    priority = dis(gen);
    left = NULL;
    right = NULL;
}

int treap::get_size() const {
    return size;
}

int treap::get_priority() const {
    return priority;
}

std::vector<int> treap::to_array() const {
    std::vector<int> answer;
    if (left != NULL) {
        const std::vector<int> lft = left->to_array();
        answer.insert(answer.end(), lft.begin(), lft.end());
    }
    answer.push_back(value);
    if (right != NULL) {
        const std::vector<int> rgt = right->to_array();
        answer.insert(answer.end(), rgt.begin(), rgt.end());
    }
    return answer;
}

int get_treap_size(const treap *current) {
    if (current == NULL) return 0;
    return current->get_size();
}

void treap::update() {
    size = get_treap_size(left) + get_treap_size(right) + 1;
}

treap* merge(treap *first, treap *last) {
    if (first == NULL) return last;
    if (last == NULL) return first;
    if (first->get_priority() > last->get_priority()) {
        first->right = merge(first->right, last);
        first->update();
        return first;
    }
    last->left = merge(first, last->left);
    last->update();
    return last;
}

void treap::split(const int size, treap *&first, treap *&last) {
    if (get_treap_size(left) + 1 == size) {
        first = this;
        last = right;
        first->right = NULL;

    } else {
        if (get_treap_size(left) >= size) {
            last = this;
            if (left) {
                left->split(size, first, last->left);
            } else {
                first = NULL;
                last->left = NULL;
            }
        } else {
            first = this;
            if (right) {
                right->split(size - get_treap_size(left) - 1, first->right, last);
            } else {
                first->right = NULL;
                last = NULL;
            }
        }
    }
    if (first) {
        first->update();
    }
    if (last) {
        last->update();
    }
}

void move(treap *&current, const int start_position, const int finish_position) {
    if (start_position == 1) return;
    treap *first, *last, *middle, *buffer;
    current->split(finish_position, buffer, last);
    buffer->split(start_position - 1, first, middle);
    buffer = merge(first, last);
    current = merge(middle, buffer);
}

void read(std::istream &input,
          int &elements_count,
          std::vector<query> &queries) {

    int queries_count;
    input >> elements_count>> queries_count;
    for (int i = 1; i <= queries_count; ++i) {
        int start_position, finish_position;
        input >> start_position >> finish_position;
        queries.push_back(query(start_position, finish_position));
    }
}

const std::vector<int> solve(const int &elements_count,
                             const std::vector<query> &queries) {

    treap *current = NULL;
    for (int i = 0; i < elements_count; ++i) {
        current = merge(current, new treap(i + 1));
    }
    for (int i = 0; i < queries.size(); ++i) {
        move(current, queries[i].get_start(), queries[i].get_finish());
    }
    return current->to_array();
}

void write(std::ostream &output,
           const std::vector<int> &answer) {

    for (int i = 0; i < answer.size(); ++i) {
        output << answer[i] << " ";
    }
}

int main() {
    int elements_count;
    std::vector<query> queries;
    read(std::cin, elements_count, queries);
    const std::vector<int> answer = solve(elements_count, queries);
    write(std::cout, answer);
    return 0;
}
