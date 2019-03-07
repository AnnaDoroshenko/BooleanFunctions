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
#include <bitset>


inline constexpr unsigned int ROWS(unsigned int parameters) { return 1 << parameters; }
inline constexpr unsigned int LAMBDA_FUNCTIONS(unsigned int parameters) { return (parameters << 1) - 2; }

template<std::size_t PARAMETERS>
class Function {
    private:

    const std::bitset<ROWS(PARAMETERS)> values;

    public:
    explicit Function(std::bitset<ROWS(PARAMETERS)>&& values);
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


    std::bitset<ROWS(PARAMETERS)> getLambdaFunction(unsigned int index) const;
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
    static Function generateNonlinearFunc();


    struct Brace {
        private:
            std::pair<unsigned int, bool> elem;

        public:
            Brace();
            Brace(unsigned int arg, bool isInverted);
            unsigned int index() const;
            bool inverted() const;
    };

    void getLinearFunction(const std::bitset<ROWS(PARAMETERS)>& currentLamdaFunc) const;
};

#include "Function.tcc"

#endif
