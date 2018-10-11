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


// Calculation of the Hamming distances between the input function
// and each of lambda functions. Then finding min distance.
// Also the closest linear functions can be shown
unsigned int Function::getMinDistance (
        const std::vector<long long>& truthTable,
        const std::vector<std::vector<unsigned int>>& arrayOfLambdaFunctions) {
    const unsigned int COUNT = arrayOfLambdaFunctions.size();
    unsigned int nonlinearity = std::numeric_limits<unsigned int>::max();
    std::vector<unsigned int> distances;
    distances.reserve(COUNT);
    for (unsigned int i = 0; i < COUNT; i++) {
        const unsigned int currentNonlinearity = [&](){
            const std::vector<unsigned int>& currentLambdaFunction = arrayOfLambdaFunctions[i];
            unsigned int currentNonlinearity = 0;
            for (unsigned int j = 0; j < currentLambdaFunction.size(); j++) {
                if (arguments[j] != currentLambdaFunction[j]) {
                    currentNonlinearity++;
                }
            }

            return currentNonlinearity;
        }();
        distances.push_back(currentNonlinearity);
        if (currentNonlinearity < nonlinearity) {
            nonlinearity = currentNonlinearity;
        }
    }
    std::vector<std::vector<unsigned int>> arrayOfLambdaFuncWithMinDistance;
    for (unsigned int i = 0; i < COUNT; i++) {
       if (distances[i] == nonlinearity) {
           arrayOfLambdaFuncWithMinDistance.push_back(arrayOfLambdaFunctions[i]);
           // For showing lambda function(-s) uncomment next lines
           const std::vector<unsigned int>& currentLFunc = arrayOfLambdaFunctions[i];
           for (unsigned int j = 0; j < currentLFunc.size(); j++) {
               std::cout << currentLFunc[j];
           }
           std::cout << std::endl;
           getLinearFunction(truthTable, currentLFunc);
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

    return getMinDistance(truthTable, arrayOfLambdaFunc);
}


// Format of a tuple is {number1, number2}
// numbers differ by one bit (for example, {100, 101})
Function::Tuple::Tuple() {
    elements = {0, 0};
}

Function::Tuple::Tuple(long long number1, long long number2) {
    elements.push_back(number1);
    elements.push_back(number2);
}

long long& Function::Tuple::operator[](unsigned int index) {
    return elements[index];
}

const long long& Function::Tuple::operator[](unsigned int index) const {
    return elements[index];
}


// Base number is a number, from which we get inverted number by one exact x.
// Base number and inverted number form tuple.
// For example, 000 - base number, 001 - number inverted by x2
std::vector<std::vector<long long>> Function::getBaseNumbers() {
    std::vector<std::vector<long long>> vectorOfBaseNumbers;
    vectorOfBaseNumbers.reserve(AMOUNT_OF_PARAMETERS);
    for (unsigned int i = 0; i < AMOUNT_OF_PARAMETERS; i++) {
        std::vector<long long> currentVectorOfBaseNumbers;
        currentVectorOfBaseNumbers.reserve(AMOUNT_OF_VARIANTS / 2);
        // step is used for jumping by base numbers' intervals
        unsigned int step = 0;
        // how many parts we have the same
        for (int j = 0; j < (1 << i); j++) {
            // workind with these parts
            for (unsigned int k = 0; k < (AMOUNT_OF_VARIANTS / (1 << (i + 1))); k++) {
                currentVectorOfBaseNumbers.push_back(step);
                step++;
            }
            step = (AMOUNT_OF_VARIANTS / (1 << i)) * (j + 1);
        }
        vectorOfBaseNumbers.push_back(currentVectorOfBaseNumbers);
    }
    
    return vectorOfBaseNumbers;
}


std::vector<Function::Tuple> Function::getTuples (
        const unsigned int& index, 
        const std::vector<long long>& baseNumbers) {
    std::vector<Tuple> tuples;
    tuples.reserve(AMOUNT_OF_VARIANTS / 2);
    const long long DIFFERENCE = 1 << index;
    for (unsigned int i = 0; i < (AMOUNT_OF_VARIANTS / 2); i++) {
        const long long number = baseNumbers[i];
        Tuple currentTuple(number, (number ^ DIFFERENCE));
        tuples.push_back(currentTuple);
        // For output of tuples in bit format uncomment next line
        /* std::cout << "{" << std::bitset<AMOUNT_OF_PARAMETERS>(currentTuple[0]) << ", " << std::bitset<AMOUNT_OF_PARAMETERS>(currentTuple[1]) << "}" << std::endl; */
        // For output of tuples in integer format uncomment next line
        /* std::cout << "{" << currentTuple[0] << ", " << currentTuple[1] << "}" << std::endl; */
    }

    return tuples;
}


void Function::getArrayOfTuples (
        std::vector<std::vector<Function::Tuple>>& arrayOfTuples) {
    arrayOfTuples.reserve(AMOUNT_OF_PARAMETERS);
    std::vector<std::vector<long long>> vectorOfBaseNumbers = getBaseNumbers();
    for (unsigned int i = 0; i < AMOUNT_OF_PARAMETERS; i++) {
        std::vector<Tuple> currentArrayOfTuples = getTuples((AMOUNT_OF_PARAMETERS - 1 - i), vectorOfBaseNumbers[i]);
        arrayOfTuples.push_back(currentArrayOfTuples);
    }
}


std::vector<double> Function::getStatistics () {
    std::vector<double> statisticArray;
    statisticArray.reserve(AMOUNT_OF_PARAMETERS);
    std::vector<std::vector<Tuple>> arrayOfTuples;
    getArrayOfTuples(arrayOfTuples);
    for (unsigned int i = 0; i < AMOUNT_OF_PARAMETERS; i++) {
        unsigned int count = 0;
        const std::vector<Tuple>& currentVectorOfArrayOfTuples = arrayOfTuples[i];
        for (unsigned int j = 0; j < (AMOUNT_OF_VARIANTS / 2); j++) {
            const Tuple& currentTuple = currentVectorOfArrayOfTuples[j];
            if (arguments[currentTuple[0]] != arguments[currentTuple[1]]) {
                count++;
            }
            /* std::cout << arguments[currentTuple[0]] << ";" << arguments[currentTuple[1]] <<std::endl; */
        }
        // To see key values of calculation uncomment next three lines
        /* std::cout << "count = " << count << std::endl; */
        /* std::cout << "AMOUNT_OF_VARIANTS = " << AMOUNT_OF_VARIANTS << std::endl; */
        /* std::cout << std::endl; */
        statisticArray[i] = static_cast<double>(count) / (AMOUNT_OF_VARIANTS / 2);
    }

    return statisticArray;
}


// method for finding linear function from lamda function
Function::Brace::Brace(unsigned int arg, bool isInverted) : elem(arg, isInverted) {}


Function::Brace::Brace() : Brace(0, false) {}


unsigned int Function::Brace::index() {
    return elem.first;
}


bool Function::Brace::inverted() {
    return elem.second;
}


void Function::getLinearFunction(
        const std::vector<long long>& truthTable, 
        const std::vector<unsigned int>& currentLamdaFunc) {
    using Argument = std::vector<Brace>;
    bool presenceTable[AMOUNT_OF_VARIANTS];
    for (unsigned int i = 0; i < AMOUNT_OF_VARIANTS; i++) {
           std::cout << "here1" << std::endl;
        if (currentLamdaFunc[i] == 1) {
            long long row = truthTable[i];
            Argument currentArg;
            currentArg.reserve(AMOUNT_OF_PARAMETERS);
            for (int j = (AMOUNT_OF_PARAMETERS - 1); j >= 0; j--) {
           std::cout << "here2" << std::endl;
                unsigned int bit = (row >> j) & 1;
                currentArg.push_back(Brace((j - AMOUNT_OF_PARAMETERS + 1), (bit == 0)));
            }
            // processing of currentArg
            for (long long combination = 0; combination < AMOUNT_OF_VARIANTS; combination++) {
           std::cout << "here3" << std::endl;
                std::vector<unsigned int> arguments;
                for (unsigned int b = 0; b < AMOUNT_OF_PARAMETERS; b++) {
           std::cout << "here4" << std::endl;
                    unsigned int currentBraceBit = (combination >> (AMOUNT_OF_PARAMETERS - 1 - b)) & 1;
                    if (currentBraceBit == 0) {
                        arguments.push_back(currentArg[b].index());
                    } else if (currentArg[b].inverted()) {
                        break;
                    }
                }
                long long currentIndex = 0;
                unsigned int currentElem = 0;
                for (unsigned int k = 0; k < AMOUNT_OF_PARAMETERS; k++) {
                    currentIndex <<= 1;
                    if (arguments[AMOUNT_OF_PARAMETERS - 1 - k] == currentElem) {
                        currentIndex++;
                    }
                    currentElem++;
                }
           std::cout << "here5" << std::endl;
                presenceTable[currentElem] = !presenceTable[currentElem];
            }
        }
    }
    // analize presenceTable and show the result
    bool notFirst = false;
    for (unsigned int index = 0; index < AMOUNT_OF_VARIANTS; index++) {
        if (presenceTable[index]) {
            if (notFirst) {
                std::cout << " + " << std::endl;
            } else {
                notFirst = true;
            }
            std::cout << "x" << index << std::endl;
        }
    }
}
