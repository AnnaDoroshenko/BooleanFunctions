#include <iostream>
#include <exception>
#include "Function.h"


int main() {
    try {
        Function testFunction("1101100001011010");
        std::cout << testFunction.calculateNonlinearity() << std::endl;
    } catch(const std::invalid_argument& e) {
        std::cerr << e.what() << std::endl;
    }

    Function testFunction2("1101100001011010");
    std::vector<double> testStat;
    testFunction2.getStatistics(testFunction2, testStat);

    for (unsigned int i = 0; i < 4; i++) {
        std::cout << i << " => " << testStat[i]  << std::endl;
    }

    return 0;
} 
