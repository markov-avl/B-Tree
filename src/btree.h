#ifndef BTREE_BTREE_H
#define BTREE_BTREE_H

#include <iostream>
#include "node.h"


template<typename data>
class BTree {
private:
    Node<data> *root;

public:
    BTree() {
        root = nullptr;
    }

    ~BTree() {
        delete root;
    }

    bool find(data key) {
        return root != nullptr && root->find(key) >= 0;
    }

    bool add(data key) {
        if (find(key)) {
            return true;
        }
        if (root == nullptr) {
            root = new Node<data>();
        }
        auto *r = root;
        if (root->num == 3) {
            auto *s = new Node<data>();
            root = s;
            s->leaf = false;
            s->num = 0;
            s->links[0] = r;
            s->splitChild(0);
            s->insertNonFull(key);
        } else {
            r->insertNonFull(key);
        }
        return false;
    }

    bool remove(data key) {
        if (root == nullptr) {
            return false;
        }
        auto flag = root->remove(key);

        if (root->num == 0) {
            auto *temp = root;
            root = temp->links[0];
            temp->links[0] = nullptr;
            delete temp;
        }

        return flag;
    }

    void print() {
        if (root != nullptr) {
            root->print(0);
        } else {
            std::cout << "B-Tree is empty";
        }
    }
};


#endif
