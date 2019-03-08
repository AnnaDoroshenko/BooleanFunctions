#ifndef FUNCTION_H
#define FUNCTION_H


#include <iostream> // std::cout
#include <vector> // std::vector
#include <string> // std::string
#include <limits> // std::numeric_limits
#include <math.h> // log2
#include <bitset> // std::bitset<numberOfBits>
#include <algorithm> // std::sort
#include <ctime> //std::time
#include <cstdlib> // std::rand
#include <cassert>
#include <thread>
#include <chrono>


// inline unsigned long fastRand() {
//     static unsigned long x = 123456789;
//     static unsigned long y = 362436069;
//     static unsigned long z = 521288629;
//     unsigned long t;
//     x ^= x << 16;
//     x ^= x >> 5;
//     x ^= x << 1;
//
//     t = x;
//     x = y;
//     y = z;
//     z = t ^ x ^ y;
//     return z;
// }

bool fastStdRand() {
    static unsigned int buffer;
    static const unsigned int roof = 1 << (sizeof(unsigned int) * 8 - 1);
    static unsigned int index = roof;
    if (index == roof) {
        index = 0;
        buffer = std::rand() % roof;
    }
    const unsigned int result = (buffer & 1) == 1;
    index++;
    buffer >>= 1;
    return result;
}

inline constexpr long long ROWS(long long parameters) { return 1 << parameters; }
inline constexpr long long LAMBDA_FUNCTIONS(long long parameters) { return (ROWS(parameters) << 1) - 2; }

template<unsigned int PARAMETERS>
class Function {
    private:

    const std::vector<bool> values;
    /* const std::bitset<ROWS(PARAMETERS)> values; */

    public:
    explicit Function(std::vector<bool>&& values);
    // explicit Function(std::bitset<ROWS(PARAMETERS)> values);

    void calculateNonlinearity() const;
    unsigned int calculateH(std::vector<unsigned int>& indeces) const;
    unsigned int calculateMinH(
        const std::array<std::pair<unsigned int, double>, PARAMETERS>&& sortedStats) const;

    private:
    bool operator[](unsigned int index) const;
    bool alreadyAdded(const std::vector<std::vector<unsigned int>>& selected, unsigned int test) const;


    struct LinearFunction {
        private:
            std::vector<unsigned int> indices;

        public:
            LinearFunction();
            LinearFunction (long long number);
            unsigned int size() const;
            unsigned int& operator[](unsigned int index);
            const unsigned int& operator[](unsigned int index) const;
    };


    std::vector<bool> getLambdaFunction(unsigned int index) const;
    /* std::bitset<ROWS(PARAMETERS)> getLambdaFunction(unsigned int index) const; */
    void getMinDistance() const;


    struct Tuple {
        private:
            long long number1;
            long long number2;

        public:
            Tuple();
            Tuple(long long number1, long long number2);
            long long& operator[](unsigned int index);
            const long long& operator[](unsigned int index) const;
    };


    std::vector<long long> getBaseNumbersForIndex(unsigned int i) const;
    long long getBaseNumber(unsigned int parameter, unsigned int index) const;
    std::vector<Tuple> getTuples (
            const unsigned int& index,
            const std::vector<long long>& baseNumbers) const;
    Tuple getTuple(long long baseNumber, unsigned int bitToInvert) const;

    void getArrayOfTuples (std::vector<std::vector<Function::Tuple>>& arrayOfTuples) const;
    std::vector<Function::Tuple> getTuplesForIndex(unsigned int i,
            const std::vector<long long>& baseNumbers) const;

    public:
    std::vector<double> getStatistics() const;
    std::array<std::pair<unsigned int, double>, PARAMETERS> getSortedStatistics() const;

    static Function<PARAMETERS> generateNonlinearFunc();

    struct Brace {
        private:
            std::pair<unsigned int, bool> elem;

        public:
            Brace();
            Brace(unsigned int arg, bool isInverted);
            unsigned int index() const;
            bool inverted() const;
    };

    // TODO
    void getLinearFunction(const std::vector<bool>& currentLamdaFunc) const;
    /* void getLinearFunction(const std::bitset<ROWS(PARAMETERS)>& currentLamdaFunc) const; */
};


template<unsigned int PARAMETERS>
// std::bitset<ROWS(PARAMETERS)> generateNonlinearFunc() {
Function<PARAMETERS> generateNonlinearFunc() {
    std::vector<bool> result(ROWS(PARAMETERS));
    // std::bitset<ROWS(PARAMETERS)> result;
    unsigned int amountOfOnes = 0;
    for (unsigned int i = 0; i < ROWS(PARAMETERS); i++) {
        /* const bool newBit = std::rand() % 2 == 0; */
        const bool newBit = fastStdRand();
        result[i] = newBit;
        if (newBit) amountOfOnes++;
    }

    // Check whether generated function is balanced.
    // Need a correction if not balanced
    const bool newBit = amountOfOnes < (ROWS(PARAMETERS) >> 1);
    while (amountOfOnes != (ROWS(PARAMETERS) >> 1)) {
        /* unsigned int pos; */
        /* do { // Keep looking until have found a wrong bit */
        /*     pos = std::rand() % ROWS(PARAMETERS); */
        /* } while (result[pos] == newBit); */
        // result[pos] = newBit;
        unsigned int pos = std::rand() % ROWS(PARAMETERS);
        while (result[pos++] == newBit && pos < ROWS(PARAMETERS)); // very clever stuff
        if (result[pos - 1] != newBit) { // if what we're looking for
            result[pos - 1] = newBit;
            amountOfOnes += newBit ? 1 : -1;
        } // else we've stopped because pos == ROWS => bad luck => try everything again
    }

    return Function<PARAMETERS>(std::move(result));
}

#include "Function.tcc"

#endif
