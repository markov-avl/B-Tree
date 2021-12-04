#ifndef STRING_NODE_H
#define STRING_NODE_H


template<typename data>
struct Node {
    int num;
    bool leaf;
    Node<data> **links;
    data *keys;

    Node() {
        num = 0;
        leaf = true;
        links = new Node<data>*[4];
        for (size_t i = 0; i < 4; ++i) {
            links[i] = nullptr;
        }
        keys = new data[3];
    }

    ~Node() {
        for (size_t i = 0; i < 4; ++i) {
            delete links[i];
        }
        delete[] links;
        delete[] keys;
    }

    int find(data key) {
        int i;
        for (i = 0; i < num; ++i) {
            if (key <= keys[i]) {
                ++i;
                break;
            }
            if (i == num - 1) {
                i = num;
            }
        }
        --i;
        if (i != num && key == keys[i]) {
            return i;
        } else if (leaf) {
            return -1;
        } else {
            return links[i]->find(key);
        }
    }

    bool remove(data key) {
        int i;
        for (i = 0; i < num; ++i) {
            if (key <= keys[i]) {
                ++i;
                break;
            }
            if (i == num - 1)
                i = num;
        }
        --i;
        if (i != num && key == keys[i]) {
            if (leaf) {
                for (int j = i; j < num - 1; ++j) {
                    keys[j] = keys[j + 1];
                }
                --num;
                keys[num] = 0;
                return true;
            } else {
                if (links[i]->num >= 2) {
                    auto nkey = getMax(i);
                    keys[i] = nkey;
                    return links[i]->remove(nkey);
                } else if (links[i + 1]->num >= 2) {
                    auto nkey = getMin(i + 1);
                    keys[i] = nkey;
                    return links[i + 1]->remove(nkey);
                } else {
                    mergeChild(i);
                    return links[i]->remove(key);
                }
            }

        } else {
            if (leaf) {
                return false;
            } else {
                if (links[i]->num == 1) {
                    if (i > 0 && links[i - 1]->num >= 2) {
                        stealLeft(i);
                    } else if (i < num && links[i + 1]->num >= 2) {
                        stealRight(i);
                    } else {
                        if (i > 0) {
                            mergeChild(i - 1);
                            --i;
                        } else {
                            mergeChild(i);
                        }
                    }
                }
                return links[i]->remove(key);
            }
        }
    }

    void print(int d) {
        for (int i = 0; i < d; i++) {
            std::cout << "        ";
        }
        std::cout << " ";
        for (int i = 0; i < num; i++) {
            std::cout << keys[i] << " ";
        }
        std::cout << std::endl;
        if (!leaf) {
            for (int i = 0; i <= num; i++) {
                links[i]->print(d + 1);
            }
        }
    }

    void splitChild(int i) {
        auto *z = new Node<data>();
        auto *y = links[i];
        z->leaf = y->leaf;
        for (int j = 0; j < 1; ++j) {
            z->keys[j] = y->keys[j + 2];
        }

        if (!y->leaf) {
            for (int j = 0; j < 2; ++j) {
                z->links[j] = y->links[j + 2];
                y->links[j + 2] = nullptr;
            }
        }

        y->num = 1;
        z->num = 1;
        for (int j = num; j > i; --j) {
            links[j + 1] = links[j];
        }
        links[i + 1] = z;
        for (int j = num - 1; j >= i; --j) {
            keys[j + 1] = keys[j];
        }
        keys[i] = y->keys[1];
        ++num;
    }

    void insertNonFull(data k) {
        int i = num - 1;
        if (leaf) {
            while (i >= 0 && k < keys[i]) {
                keys[i + 1] = keys[i];
                --i;
            }
            keys[i + 1] = k;
            ++num;
        } else {
            while (i >= 0 && k < keys[i]) {
                --i;
            }
            ++i;
            if (links[i]->num == 3) {
                splitChild(i);
                if (k > keys[i]) {
                    ++i;
                }
            }
            links[i]->insertNonFull(k);
        }
    }

    void mergeChild(int i) {
        auto *y = links[i];
        auto *z = links[i + 1];
        auto yn = y->num;
        auto zn = z->num;

        y->keys[yn] = keys[i];
        for (int j = 0; j < zn; ++j) {
            y->keys[yn + j + 1] = z->keys[j];
        }
        if (!y->leaf)
            for (int j = 0; j <= zn; ++j) {
                y->links[yn + 1 + j] = z->links[j];
                z->links[j] = nullptr;
            }
        y->num = yn + zn + 1;
        delete z;
        for (int j = i + 1; j < num; ++j) {
            links[j] = links[j + 1];
        }
        links[num] = nullptr;
        for (int j = i; j < num - 1; ++j) {
            keys[j] = keys[j + 1];
        }
        keys[num - 1] = 0;
        --num;
    }

    void stealLeft(int i) {
        auto *x = links[i];
        auto *y = links[i - 1];
        auto k = keys[i - 1];
        auto xn = x->num;
        auto yn = y->num;

        for (int j = xn; j > 0; --j) {
            x->keys[j] = x->keys[j - 1];
        }
        x->keys[0] = k;
        keys[i - 1] = y->keys[yn - 1];
        y->keys[yn - 1] = 0;

        for (int j = xn; j >= 0; --j) {
            x->links[j + 1] = x->links[j];
        }
        x->links[0] = y->links[yn];
        y->links[yn] = nullptr;

        x->num += 1;
        y->num -= 1;
    }

    void stealRight(int i) {
        auto *x = links[i];
        auto *y = links[i + 1];
        auto k = keys[i];
        auto xn = x->num;
        auto yn = y->num;

        x->keys[xn] = k;
        keys[i] = y->keys[0];
        for (int j = 0; j < yn - 1; ++j) {
            y->keys[j] = y->keys[j + 1];
        }
        y->keys[yn - 1] = 0;

        x->links[xn + 1] = y->links[0];
        for (int j = 0; j < yn; ++j) {
            y->links[j] = y->links[j + 1];
        }
        y->links[yn] = nullptr;

        x->num += 1;
        y->num -= 1;
    }

    data getMin(int i) {
        auto *x = links[i];
        while (!x->leaf) {
            x = x->links[0];
        }
        return x->keys[0];
    }

    data getMax(int i) {
        auto *x = links[i];
        while (!x->leaf) {
            x = x->links[x->num];
        }
        return x->keys[x->num - 1];
    }
};


#endif
