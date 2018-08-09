#include "Function.h"


// Linear function format is x(n),..x(1),x(0),1
LinearFunction::LinearFunction() {};
LinearFunction::LinearFunction(long long number) {
    // Extra one iteration is added to include "1" as element of linear equation
    for (unsigned int i = 0; i <= AMOUNT_OF_PARAMETERS; i++) {
        if ((number & 1) == 1) {
            indices.push_back(AMOUNT_OF_PARAMETERS - i);
        }
        number >>= 1;
    }
};

unsigned int LinearFunction::size() const {
    return indices.size();
}

unsigned int& LinearFunction::operator[](unsigned int index) {
    return indices[index];
}

const unsigned int& LinearFunction::operator[](unsigned int index) const {
    return indices[index];
}


Function::Function(std::string input) : 
    AMOUNT_OF_PARAMETERS(log2(input.size())),
    AMOUNT_OF_VARIANTS(input.size()),
    AMOUNT_OF_LAMBDA_FUNCTIONS(AMOUNT_OF_VARIANTS * 2 - 2) {
        unsigned int s = input.size();
        while ((s & 1) == 0) s >>= 1;
        if ((s >> 1) != 0) {
            throw std::invalid_argument("Check amount of arguments of the function");
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


// Method for generation of the Truth Table
void Function::generateTruthTable (std::vector<long long>& truthTable) {
    truthTable.reserve(AMOUNT_OF_VARIANTS);
    for (unsigned int i = 0; i < AMOUNT_OF_VARIANTS; i++) {
        truthTable.push_back(i);
    }
}


// Method for generation of the array of linear functions
void Function::getArrayOfLinearFunctions (std::vector<LinearFunction>& array) {
    array.reserve(AMOUNT_OF_LAMBDA_FUNCTIONS);
    // Starts with i = 2, because i is used for generation of linear functions
    // and functions f = 0 ==> [!x(n),..!x(1),!x(0),!1]
    // and f = 1 ==> [!x(n),..!x(1),!x(0),1] are not included 
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
        if (linearFunction[0] == AMOUNT_OF_PARAMETERS) {
            currentResult = 1;
            j++;
        }
        for ( ; j < linearFunction.size(); j++) {
            if ((truthTable[i] >> (AMOUNT_OF_PARAMETERS - 1 - linearFunction[j])) & 1) {
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
