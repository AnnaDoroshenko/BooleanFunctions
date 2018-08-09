#include <iostream>
#include <exception>
#include "Function.h"


int main() {
    try {
        Function testFunction("11001001");
        std::cout << testFunction.calculateNonlinearity() << std::endl;
    } catch(const std::invalid_argument& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
} 
