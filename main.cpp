#include <iostream>
#include "all.h"
using namespace cl;

int main(int argc, char *argv[]) {
    auto t = make_shared<int>(19);
    std::cout << *t << std::endl;

    auto b = make_unique<int>(10);
    std::cout << *b << std::endl;

    auto c = std::move(b);
    std::cout << *c << std::endl;

    if (b.get() == nullptr) printf("oK");
}
