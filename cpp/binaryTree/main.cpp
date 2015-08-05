#include "binaryTree.h"
#include <iostream>
#include <cassert>
#include <cstdlib>
#include <ctime>
#include <chrono>
using namespace std::chrono;

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

    int large = 2000000;
    auto t1 = std::chrono::steady_clock::now();
    for (auto i = 0; i < large; i++) {
        tree.set(std::rand() % large, 1000);
    }

    auto interval = duration_cast<milliseconds>(steady_clock::now() - t1).count();
    std::cout << "insert cost " << interval << std::endl;

    std::cout << "tree size : " << tree.size() << std::endl;

    t1 = steady_clock::now();
    tree.enumerate([](const int key, double& value) {
        double v = value;
    });
    interval = duration_cast<milliseconds>(steady_clock::now() - t1).count();
    std::cout << "enumerate cost " << interval << std::endl;

    t1 = steady_clock::now();
    for (auto i = 0; i < large; i++) {
        tree.exist(i);
    }
    interval = duration_cast<milliseconds>(steady_clock::now() - t1).count();
    std::cout << "find cost " << interval << std::endl;
    return 0;
}
