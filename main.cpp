#include "src/btree.h"


int main() {
    BTree<int> bTree;

    bTree.print();

    bTree.add(45);
    bTree.add(12);
    bTree.remove(11);
    bTree.add(98);
    bTree.add(35);
    bTree.add(9);

    bTree.print();

    bTree.remove(98);
    bTree.remove(9);

    bTree.print();

    return 0;
}