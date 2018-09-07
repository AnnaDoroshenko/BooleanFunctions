#include <iostream>
#include <exception>
#include "Function.h"


int main() {

    Function testFunction("1101100001011010");

    try {
        std::cout << "Nonlinearity = ";
        std::cout << testFunction.calculateNonlinearity() << std::endl;
    } catch(const std::invalid_argument& e) {
        std::cerr << e.what() << std::endl;
    }

    std::vector<double> testStat = testFunction.getStatistics();

    for (unsigned int i = 0; i < 4; i++) {
        std::cout << i << " => " << testStat[i]  << std::endl;
    }

    return 0;
} 
