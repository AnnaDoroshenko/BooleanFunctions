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



class Function {
    const unsigned int AMOUNT_OF_PARAMETERS;
    const unsigned int AMOUNT_OF_VARIANTS;
    const unsigned int AMOUNT_OF_LAMBDA_FUNCTIONS;
    std::vector<unsigned int> arguments;

    public:
    Function();
    Function(std::string input);
    /* unsigned int calculateNonlinearity(); */
    void calculateNonlinearity();
    unsigned int calculateH(std::vector<unsigned int>& indeces);
    unsigned int calculateMinH(std::vector<std::pair<unsigned int, double>>& sortedStats);

    private:
    unsigned int& operator[](unsigned int index);
    const unsigned int& operator[](unsigned int index) const;
    bool alreadyAdded(std::vector<std::vector<unsigned int>>& selected, unsigned int& test);


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


    /* void generateTruthTable (std::vector<long long>& truthTable); */
    void getArrayOfLinearFunctions (std::vector<LinearFunction>& array);
    void getLambdaFunction (
            const LinearFunction& linearFunction,
            std::vector<unsigned int>& lambdaFunction);
    void getArrayOfLambdaFunctions (
            const std::vector<LinearFunction>& arrayOfLinearFunctions,
            std::vector<std::vector<unsigned int>>& arrayOfLambdaFunctions);
    /* unsigned int getMinDistance ( */
    /*         const std::vector<long long>& trurhTable, */
    /*         const std::vector<std::vector<unsigned int>>& arrayOfLambdaFunctions); */
    void getMinDistance (
            const std::vector<std::vector<unsigned int>>& arrayOfLambdaFunctions);


    struct Tuple {
        private:
            /* std::vector<long long> elements; */
            long long number1;
            long long number2;

        public:
            Tuple();
            Tuple(long long number1, long long number2);
            long long& operator[](unsigned int index);
            const long long& operator[](unsigned int index) const;
    };


    std::vector<long long> getBaseNumbersForIndex(unsigned int i);
    std::vector<Tuple> getTuples (
            const unsigned int& index,
            const std::vector<long long>& baseNumbers);
    void getArrayOfTuples (std::vector<std::vector<Function::Tuple>>& arrayOfTuples);
    std::vector<Function::Tuple> getTuplesForIndex(unsigned int i,
            const std::vector<long long>& baseNumbers);

    public:
    std::vector<double> getStatistics ();
    std::vector<std::pair<unsigned int, double>> getSortedStatistics();
    static std::string generateNonlinearFunc(unsigned int n);


    struct Brace {
        private:
            std::pair<unsigned int, bool> elem;

        public:
            Brace();
            Brace(unsigned int arg, bool isInverted);
            unsigned int index();
            bool inverted();
    };


    void getLinearFunction(
            const std::vector<unsigned int>& currentLamdaFunc);
};


#endif
