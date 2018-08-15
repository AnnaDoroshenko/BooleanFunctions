#include "Function.h"


Function::Function(std::string input) : 
    AMOUNT_OF_PARAMETERS(log2(input.size())),
    AMOUNT_OF_VARIANTS(input.size()),
    AMOUNT_OF_LAMBDA_FUNCTIONS(AMOUNT_OF_VARIANTS * 2 - 2) {
        unsigned int s = input.size();
        while ((s & 1) == 0) s >>= 1;
        if ((s >> 1) != 0) {
            throw std::invalid_argument("Invalid amount of function arguments passed. Terminating");
        }
        for (unsigned int i = 0; i < AMOUNT_OF_VARIANTS; i++) {
            const unsigned int currentArgument = input.at(i) - '0';
            arguments.push_back(currentArgument);
        }
}

unsigned int& Function::operator[](unsigned int index) {
    return arguments[index];
}

const unsigned int& Function::operator[](unsigned int index) const {
    return arguments[index];
}


// Generation of the Truth Table
void Function::generateTruthTable (std::vector<long long>& truthTable) {
    truthTable.reserve(AMOUNT_OF_VARIANTS);
    for (unsigned int i = 0; i < AMOUNT_OF_VARIANTS; i++) {
        truthTable.push_back(i);
    }
}


// Linear function format is x(0),x(1),..x(n) [1,x(0),x(1),..x(n)]
Function::LinearFunction::LinearFunction() {};
Function::LinearFunction::LinearFunction(long long number) {
    unsigned int index = 0;
    while (number != 0) {
        if ((number & 1) == 1) {
            indices.push_back(index);
        }
        number >>= 1;
        index++;
    }
};

unsigned int Function::LinearFunction::size() const {
    return indices.size();
}

unsigned int& Function::LinearFunction::operator[](unsigned int index) {
    return indices[index];
}

const unsigned int& Function::LinearFunction::operator[](unsigned int index) const {
    return indices[index];
}


// Generation of the array of linear functions
void Function::getArrayOfLinearFunctions (std::vector<LinearFunction>& array) {
    array.reserve(AMOUNT_OF_LAMBDA_FUNCTIONS);
    // Starts with i = 2, because i is used for generation of linear functions
    // and functions f = 0 ==> [!1,!x(0),!x(1),..!x(n)]
    // and f = 1 ==> [1,!x(0),!x(1),..!x(n)] are not included 
    for (unsigned int i = 2; i < (AMOUNT_OF_LAMBDA_FUNCTIONS + 2); i++){
        array.push_back(LinearFunction(i));
    }
}


void Function::getLambdaFunction (
        const std::vector<long long>& truthTable, 
        const LinearFunction& linearFunction, 
        std::vector<unsigned int>& lambdaFunction) {
    lambdaFunction.reserve(AMOUNT_OF_VARIANTS);
    for (unsigned int i = 0; i < AMOUNT_OF_VARIANTS; i++) {
        unsigned int currentResult = 0;
        unsigned int j = 0;
        if (linearFunction[0] == 0) {
            currentResult = 1;
            j++;
        }
        for ( ; j < linearFunction.size(); j++) {
            if ((truthTable[i] >> (linearFunction[j] - 1)) & 1) {
                currentResult ^= 1;
            }
        }
        lambdaFunction.push_back(currentResult);
    }
}


void Function::getArrayOfLambdaFunctions (
        const std::vector<long long>& truthTable, 
        const std::vector<LinearFunction>& arrayOfLinearFunctions, 
        std::vector<std::vector<unsigned int>>& arrayOfLambdaFunctions) {
    arrayOfLambdaFunctions.reserve(AMOUNT_OF_LAMBDA_FUNCTIONS);
    for (unsigned int i = 0; i < AMOUNT_OF_LAMBDA_FUNCTIONS; i++) {
        std::vector<unsigned int> currentLambdaFunction;
        currentLambdaFunction.reserve(AMOUNT_OF_VARIANTS);
        getLambdaFunction(truthTable, arrayOfLinearFunctions[i], currentLambdaFunction);
        arrayOfLambdaFunctions.push_back(currentLambdaFunction);
    }
}


// Method for calculation of the Hamming distance between the input function
// and each of lambda functions and finding min of them
unsigned int Function::getMinDistance (
        const std::vector<std::vector<unsigned int>>& arrayOfLambdaFunctions) {
    unsigned int nonlinearity = std::numeric_limits<unsigned int>::max();
        for (unsigned int i = 0; i < arrayOfLambdaFunctions.size(); i++) {
        unsigned int currentNonlinearity = 0;
        const std::vector<unsigned int>& currentLambdaFunction = arrayOfLambdaFunctions[i];
        for (unsigned int j = 0; j < currentLambdaFunction.size(); j++) {
            if (arguments[j] != currentLambdaFunction[j]) {
                currentNonlinearity++;
            }
        }
        if (currentNonlinearity < nonlinearity) {
            nonlinearity = currentNonlinearity;
        }
    }

    return nonlinearity;
}


unsigned int Function::calculateNonlinearity() {
    std::vector<long long> truthTable;
    generateTruthTable(truthTable);

    std::vector<LinearFunction> arrayOfLinFunc;
    getArrayOfLinearFunctions(arrayOfLinFunc);

    std::vector<std::vector<unsigned int>> arrayOfLambdaFunc;
    arrayOfLambdaFunc.reserve(AMOUNT_OF_LAMBDA_FUNCTIONS);

    getArrayOfLambdaFunctions(truthTable, arrayOfLinFunc, arrayOfLambdaFunc);

    return getMinDistance(arrayOfLambdaFunc);
}
