#include <iostream>
#include <exception>
#include "Function.h"


int main() {

    std::srand(std::time(nullptr));
    const unsigned int N = 16;
    const unsigned int STATISTIC_AMOUNT = 10;
    std::vector<std::string> testFunctions;
    testFunctions.reserve(STATISTIC_AMOUNT);
    for (unsigned int count = 0; count < STATISTIC_AMOUNT; count++) {
        Function generateNLF;
        std::string funcString = generateNLF.generateNonlinearFunc(N);
        testFunctions.push_back(funcString);
    }

    /* for (unsigned int i = 0; i < STATISTIC_AMOUNT; i++) { */
    /*     std::cout << testFunctions[i] << std::endl; */
    /* } */

    std::vector<unsigned int> nonlinearityStat;
    nonlinearityStat.reserve(STATISTIC_AMOUNT);
    for (unsigned int i = 0; i < testFunctions.size(); i++) {
        std::cout << "---------------- " << i <<" ------------------" << std::endl;
        Function testFunction(testFunctions[i]);

        try {
            testFunction.calculateNonlinearity();
        } catch(const std::invalid_argument& e) {
            std::cerr << e.what() << std::endl;
        }

        std::vector<std::pair<unsigned int, double>> testStat = testFunction.getSortedStatistics();
        unsigned int currN = testFunction.calculateMinH(testStat);
        nonlinearityStat.push_back(currN);
        std::cout << "======================================" << std::endl;
        std::cout << "======================================" << std::endl;
        /* std::vector<unsigned int> index = {1}; */
        /* testFunction.calculateH(index); */
    }

    unsigned int sum = 0;
    for (unsigned int k = 0; k < STATISTIC_AMOUNT; k++) {
        sum += nonlinearityStat.at(k);
    }
    unsigned int mean = static_cast<unsigned int>(sum / STATISTIC_AMOUNT) + 1; 
    std::cout << "mean = " << mean << std::endl;

    return 0;
} 
