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
    tree.set(5, 2.5);
    tree.remove(3);
    assert(!tree.exist(3));
    assert(tree.size() == 2);
    assert(tree.find(5) - 2.5 < 0.000000001);

    std::srand(std::time(0));

    long large = 2000000;
    auto t1 = std::chrono::steady_clock::now();
    for (auto i = 0; i < large; i++) {
        tree.set(std::rand() % large, 1000);
    }

    auto interval = duration_cast<milliseconds>(steady_clock::now() - t1).count();
    std::cout << "insert cost " << interval << std::endl;

    std::cout << "tree size : " << tree.size() << std::endl;

    t1 = steady_clock::now();
    tree.enumerate([](const int key, double& value) {
//        std::cout << key << " : " << value << std::endl;
    });
    interval = duration_cast<milliseconds>(steady_clock::now() - t1).count();
    std::cout << "enumerate cost " << interval << std::endl;

    t1 = steady_clock::now();
    for (auto i = 0; i < large; i++) {
        tree.exist(i);
    }
    interval = duration_cast<milliseconds>(steady_clock::now() - t1).count();
    std::cout << "find cost " << interval << std::endl;

    t1 = steady_clock::now();
    for (auto i = 0; i < large; i++) {
        tree.remove(i);
    }
    interval = duration_cast<milliseconds>(steady_clock::now() - t1).count();
    std::cout << "remove const " << interval << std::endl;
    assert(tree.size() == 0);
    return 0;
}
