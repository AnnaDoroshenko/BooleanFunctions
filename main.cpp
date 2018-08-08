#include <iostream> // std::cout;
#include <vector> // std::vector
#include <limits> // std::numeric_limits


static const unsigned int AMOUNT_OF_PARAMETERS = 3;
static const unsigned int AMOUNT_OF_VARIANTS = 1 << AMOUNT_OF_PARAMETERS; 
static const unsigned int AMOUNT_OF_LAMBDA_FUNCTIONS = AMOUNT_OF_VARIANTS * 2 - 2;


struct Function {
    private:
        // bool overflow;
        std::vector<unsigned int> elements;

    public:
        Function() {
            for (unsigned int i = 0; i < AMOUNT_OF_PARAMETERS; i++) {
                elements.push_back(0);
            }
        }

        // Function is represented in the binary format of a number
        Function (unsigned int function) {
            for (unsigned int i = 0; i < AMOUNT_OF_VARIANTS; i++) {
                unsigned int currentBit = function >> (AMOUNT_OF_VARIANTS - 1 - i);
                elements.push_back(currentBit & 1);
            }
        }

        unsigned int& operator[](unsigned int index) {
            return elements[index];
        }

        const unsigned int& operator[](unsigned int index) const {
            return elements[index];
        }
};


// Linear function format is x(n),..x(1),x(0),1
struct LinearFunction {
    private:
        std::vector<unsigned int> elements;

    public:
        LinearFunction() {}

        LinearFunction (long long number) {
            // Extra one iteration is added to include "1" as element of linear equation
            for (unsigned int i = 0; i <= AMOUNT_OF_PARAMETERS; i++) {
                if ((number & 1) == 1) {
                    elements.push_back(AMOUNT_OF_PARAMETERS - i);
                }
                number >>= 1;
            }
        }

        unsigned int size() const{
            return elements.size();
        }

        unsigned int& operator[](unsigned int index) {
            return elements[index];
        }

        const unsigned int& operator[](unsigned int index) const {
            return elements[index];
        }
};


// Method for generation of the Truth Table
void generateTruthTable (std::vector<long long>& truthTable) {
    truthTable.reserve(AMOUNT_OF_VARIANTS);
    for (unsigned int i = 0; i < AMOUNT_OF_VARIANTS; i++) {
        truthTable.push_back(i);
    }
}


// Method for generation of the array of linear functions
void getArrayOfLinearFunctions (std::vector<LinearFunction>& array) {
    array.reserve(AMOUNT_OF_LAMBDA_FUNCTIONS);
    // Starts with i = 2, because i is used for generation of linear functions
    // and functions f = 0 ==> [!x(n),..!x(1),!x(0),!1]
    // and f = 1 ==> [!x(n),..!x(1),!x(0),1] are not included 
    for (unsigned int i = 2; i < (AMOUNT_OF_LAMBDA_FUNCTIONS + 2); i++){
        array.push_back(LinearFunction(i));
    }
}


void getLambdaFunction (const std::vector<long long>& truthTable, const LinearFunction& linearFunction, std::vector<unsigned int>& lambdaFunction) {
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


void getArrayOfLambdaFunctions (const std::vector<long long>& truthTable, const std::vector<LinearFunction>& arrayOfLinearFunctions, std::vector<std::vector<unsigned int>>& arrayOfLambdaFunctions) {
    arrayOfLambdaFunctions.reserve(AMOUNT_OF_LAMBDA_FUNCTIONS);
    for (unsigned int i = 0; i < AMOUNT_OF_LAMBDA_FUNCTIONS; i++) {
        std::vector<unsigned int> currentLambdaFunction;
        currentLambdaFunction.reserve(AMOUNT_OF_VARIANTS);
        getLambdaFunction(truthTable, arrayOfLinearFunctions[i], currentLambdaFunction);
        arrayOfLambdaFunctions.push_back(currentLambdaFunction);
    }
}


unsigned int getNonlinearityOfFunction (const Function& function, const std::vector<std::vector<unsigned int>>& arrayOfLambdaFunctions) {
    unsigned int nonlinearity = std::numeric_limits<unsigned int>::max();
        for (unsigned int i = 0; i < arrayOfLambdaFunctions.size(); i++) {
        unsigned int currentNonlinearity = 0;
        const std::vector<unsigned int>& currentLambdaFunction = arrayOfLambdaFunctions[i];
        for (unsigned int j = 0; j < currentLambdaFunction.size(); j++) {
            if (function[j] != currentLambdaFunction[j]) {
                currentNonlinearity++;
            }
        }
        if (currentNonlinearity < nonlinearity) {
            nonlinearity = currentNonlinearity;
        }
    }

    return nonlinearity;
}


int main() {

    std::vector<long long> truthTable;
    generateTruthTable(truthTable);

    std::vector<LinearFunction> arrayOfLinFunc;
    getArrayOfLinearFunctions(arrayOfLinFunc);

    std::vector<std::vector<unsigned int>> arrayOfLambdaFunc;
    arrayOfLambdaFunc.reserve(AMOUNT_OF_LAMBDA_FUNCTIONS);

    getArrayOfLambdaFunctions(truthTable, arrayOfLinFunc, arrayOfLambdaFunc);

    Function testFunc(0b11000101);
    unsigned int res = getNonlinearityOfFunction(testFunc, arrayOfLambdaFunc);
    std::cout << res << std::endl;

    return 0;
} 
