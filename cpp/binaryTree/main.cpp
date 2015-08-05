#include "binaryTree.h"
#include <iostream>
#include <cassert>
#include <cstdlib>
#include <ctime>

int main(int , char **)
{
    BinaryTree<int, double> tree;
    tree.set(3, 2.3);
    tree.set(4, 5.6);
    tree.set(5, 3.2);
    tree.set(1, 2.5);
    tree.remove(4);
    assert(!tree.exist(4));
    assert(tree.size() == 3);

    std::srand(std::time(0));

    for (auto i = 0; i < 20; i++) {
        tree.set(std::rand() % 200, std::rand() % 1000);
    }

    tree.enumerate([](const int key, double& value) {
        std::cout << key << " : " << value << std::endl;
    });

    return 0;
}
