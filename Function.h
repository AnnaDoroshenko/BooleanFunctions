#ifndef FUNCTION_H
#define FUNCTION_H


#include <iostream> // std::cout;
#include <vector> // std::vector
#include <string> // std::string
#include <limits> // std::numeric_limits
#include <math.h> // log2


class Function {
    const unsigned int AMOUNT_OF_PARAMETERS;
    const unsigned int AMOUNT_OF_VARIANTS;
    const unsigned int AMOUNT_OF_LAMBDA_FUNCTIONS;
    std::vector<unsigned int> arguments;

    Function(std::string input);
    unsigned int& operator[](unsigned int index);
    const unsigned int& operator[](unsigned int index) const;


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


    void generateTruthTable (std::vector<long long>& truthTable);
    void getArrayOfLinearFunctions (std::vector<LinearFunction>& array);
    void getLambdaFunction (
            const std::vector<long long>& truthTable,
            const LinearFunction& linearFunction,
            std::vector<unsigned int>& lambdaFunction);
    void getArrayOfLambdaFunctions (
            const std::vector<long long>& truthTable, 
            const std::vector<LinearFunction>& arrayOfLinearFunctions, 
            std::vector<std::vector<unsigned int>>& arrayOfLambdaFunctions);
    unsigned int getMinDistance (
            const std::vector<std::vector<unsigned int>>& arrayOfLambdaFunctions);

    public:
    unsigned int calculateNonlinearity();
};


#endif
