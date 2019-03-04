#include "Function.h"


Function::Function() :
    AMOUNT_OF_PARAMETERS(0),
    AMOUNT_OF_VARIANTS(0),
    AMOUNT_OF_LAMBDA_FUNCTIONS(0) {}


Function::Function(std::string input) : 
    AMOUNT_OF_PARAMETERS(log2(input.size())),
    AMOUNT_OF_VARIANTS(input.size()),
    AMOUNT_OF_LAMBDA_FUNCTIONS(AMOUNT_OF_VARIANTS * 2 - 2) {
        std::cout << "--------------------------------------" << std::endl;
        std::cout << "Input: " << input << std::endl;
        std::cout << "--------------------------------------" << std::endl;

        unsigned int s = input.size();
        while ((s & 1) == 0) s >>= 1;
        if ((s >> 1) != 0) {
            throw std::invalid_argument("Invalid amount of function arguments passed. Terminating");
        }

        unsigned int countOnes = 0;
        unsigned int countZeros = 0;
        for (unsigned int bit = 0; bit < input.size(); bit++) {
            if (input[bit] == '1') {
                countOnes++;
            } else {
                countZeros++;
            }
        }
        if (countOnes != countZeros) {
            std::cout << "countOnes = " << countOnes << std::endl;
            std::cout << "countZeros = " << countZeros << std::endl;
            throw std::invalid_argument("Input function is not balanced. Terminating");
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


// Calculates Hamming distance between the input functions
// (and Truth Table representation of linear function can be printed)
// Be careful about input: [x(0), x(1), .. x(n), 1]
// For example, f=x3 => 0...00100, so function expects 4
unsigned int Function::calculateH(
        std::vector<unsigned int>& indeces) {
    unsigned int decForm = 0;
    for (unsigned int index : indeces) {
        decForm += 1 << (AMOUNT_OF_PARAMETERS - index);
    }
    LinearFunction linearFunction = LinearFunction(decForm);
    std::vector<long long> truthTable;
    generateTruthTable(truthTable);
    std::vector<unsigned int> lambdaFunction;
    lambdaFunction.reserve(AMOUNT_OF_VARIANTS);
    getLambdaFunction(truthTable, linearFunction, lambdaFunction);

    // For Truth Table representation uncomment next loop
    /* for (unsigned int i = 0; i < lambdaFunction.size(); i++) { */
    /*     std::cout << lambdaFunction[i]; */
    /* } */
    /* std::cout << std::endl; */

    unsigned int nonlinearity = 0;
    for (unsigned int j = 0; j < lambdaFunction.size(); j++) {
        if (arguments[j] != lambdaFunction[j]) {
            nonlinearity++;
        }
    }
    if (nonlinearity > (AMOUNT_OF_VARIANTS / 2)) {
        nonlinearity = AMOUNT_OF_VARIANTS - nonlinearity;
    }
    /* std::cout << " H = " << nonlinearity << std::endl; */

    return nonlinearity;
}

unsigned int Function::calculateMinH(
        std::vector<std::pair<unsigned int, double>>& sortedStats) {
    std::vector<unsigned int> sorted;
    std::vector<unsigned int> distances;
    sorted.reserve(AMOUNT_OF_PARAMETERS);
    distances.reserve(AMOUNT_OF_PARAMETERS);
    for (std::pair<unsigned int, double> stat : sortedStats) {
        sorted.push_back(stat.first);
    }
    std::cout << "sorted = [ ";
    for (unsigned int s : sorted) {
        std::cout << "x" << s << " ";
    }
    std::cout << "]" << std::endl;
    std::cout << "======================================" << std::endl;

    std::vector<std::vector<unsigned int>> selected;
    selected.push_back({sorted[0]});
    unsigned int minH = calculateH(selected[0]);
    distances.push_back(minH);
    unsigned int selectedSize = 1;
    /* std::cout << "k = 0, m = " << selectedSize; */
    /* for (std::vector<unsigned int> sel : selected) { */
    /*     std::cout << " l0 = "; */
    /*     for (unsigned int s : sel) { */
    /*         std::cout << "x" << s << " "; */
    /*     } */
    /*     std::cout << std::endl; */
    /* } */
    unsigned int passes = 1;
    for(unsigned int k = 1; k < sortedStats.size(); k++) {
        /* std::cout << "--------------------------------------" << std::endl; */
        /* std::cout << "k = " << k << std::endl; */
        for (unsigned int h = 0, a = 0; h < (selectedSize - a); h++) {
            /* std::cout << "--------------------------------------" << std::endl; */
            /* std::cout << "h = " << h << " a = " << a << std::endl; */
            /* unsigned int h1 = calculateH(selected[h]); */
            unsigned int h1 = distances.at(h);
            if (h1 < minH) {
                minH = h1;
            }
            std::vector<unsigned int> sel = selected[h];
            sel.push_back(sorted[k]);
            /* for (unsigned int s: sel) { */
            /*     std::cout << s << std::endl; */
            /* } */
            unsigned int h2 = calculateH(sel);
            if (h2 < minH) {
                minH = h2;
            }
            /* std::cout << "H ( "; */
            /* for (unsigned int s : selected[h]) { */
            /*     std::cout << "x" << s << " "; */
            /* } */
            /* std::cout << ") = " << h1 << std::endl; */
            /* std::cout << "H ( "; */
            /* for (unsigned int s : sel) { */
            /*     std::cout << "x" << s << " "; */
            /* } */
            /* std::cout << ") = " << h2 << std::endl; */
            passes++;
            if (h2 <= h1) {
                selected[h] = std::move(sel);
                distances.at(h) = h2;
            } else if (!alreadyAdded(selected, sorted[k])) {
                selected.push_back({sorted[k]});
                distances.push_back(calculateH(selected[selected.size()-1]));
                selectedSize++;
                a++;
            }
            /* for (unsigned int i = 0; i < selected.size(); i++) { */
            /*     std::cout << "l" << i << " = "; */
            /*     for (unsigned int s : selected[i]) { */
            /*         std::cout << "x" << s << " "; */
            /*     } */
            /*     std::cout << std::endl; */
            /* } */
            /* std::cout << "m = " << selectedSize << std::endl; */
        }
    }
    /* for (unsigned int distance : distances) { */
    /*     std::cout << " " << distance; */
    /* } */
    // std::cout << std::endl;
    std::cout << "amount of args = " << AMOUNT_OF_PARAMETERS << std::endl;
    std::cout << "nonlinearity = " << minH << std::endl;
    /* std::cout << "passes = " << passes << std::endl; */
    unsigned int realPasses = AMOUNT_OF_PARAMETERS + passes;
    std::cout << "passes = " << realPasses << std::endl;
    
    return realPasses;
    /* return sorted; */
}


bool Function::alreadyAdded(
        std::vector<std::vector<unsigned int>>& selected, unsigned int& test) {
    for (std::vector<unsigned int> sel : selected) {
        if ((sel.size() == 1) && (sel[0] == test)) { return true; }
    }

    return false;
}


// Calculation of the Hamming distances between the input function
// and each of lambda functions. Then finding min distance.
// Also the closest linear functions can be shown
/* unsigned int Function::getMinDistance ( */
void Function::getMinDistance (
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
    std::cout << "Nonlinearity = " << nonlinearity << std::endl;
    std::cout << "--------------------------------------" << std::endl;

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

    /* return nonlinearity; */
}


/* unsigned int Function::calculateNonlinearity() { */
void Function::calculateNonlinearity() {
    std::vector<long long> truthTable;
    generateTruthTable(truthTable);

    std::vector<LinearFunction> arrayOfLinFunc;
    getArrayOfLinearFunctions(arrayOfLinFunc);

    std::vector<std::vector<unsigned int>> arrayOfLambdaFunc;
    arrayOfLambdaFunc.reserve(AMOUNT_OF_LAMBDA_FUNCTIONS);

    getArrayOfLambdaFunctions(truthTable, arrayOfLinFunc, arrayOfLambdaFunc);

    /* return getMinDistance(truthTable, arrayOfLambdaFunc); */
    getMinDistance(truthTable, arrayOfLambdaFunc);
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


std::vector<double> Function::getStatistics() {
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
        statisticArray.push_back(static_cast<double>(count) / (AMOUNT_OF_VARIANTS / 2));
        std::cout << "x" << i << " => " << statisticArray[i] << std::endl;
    }
    std::cout << "--------------------------------------" << std::endl;

    return statisticArray;
}


std::vector<std::pair<unsigned int, double>> Function::getSortedStatistics() {
    using Interest = std::pair<unsigned int, double>;
    std::vector<Interest> statisticArray;
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
        }
        statisticArray.push_back(Interest(i, static_cast<double>(count) / (AMOUNT_OF_VARIANTS / 2)));
    }
    std::sort(statisticArray.begin(), statisticArray.end(), [](const Interest &left, const Interest &right) {
            return right.second < left.second;
            });

    for (Interest stat : statisticArray) {
        std::cout << "x" << stat.first << " = " << stat.second << std::endl;
    }
    std::cout << "--------------------------------------" << std::endl;
    
    return statisticArray;

    // To get statistic from variables, probabilities of which are greater than 0.3
    /* std::vector<Interest> statisticTransformedArray; */
    /* for (unsigned int i = 0; i < statisticArray.size(); i++) { */
    /*     Interest& statistic = statisticArray[i]; */
    /*     if(statistic.second > 0.3) { */
    /*         statisticTransformedArray.push_back(statisticArray[i]); */
    /*     } */
    /* } */
    /*  */
    /* for (Interest stat : statisticTransformedArray) { */
    /*     std::cout << "x" << stat.first << " = " << stat.second << std::endl; */
    /* } */
    /* std::cout << "--------------------------------------" << std::endl; */
    /*  */
    /* return statisticTransformedArray; */
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
    for (unsigned int i = 0; i < AMOUNT_OF_VARIANTS; i++) presenceTable[i] = false;

    for (unsigned int i = 0; i < AMOUNT_OF_VARIANTS; i++) {
        if (currentLamdaFunc[i] == 0) continue;
        long long row = truthTable[i];
        Argument currentArg;
        currentArg.reserve(AMOUNT_OF_PARAMETERS);
        for (int j = (AMOUNT_OF_PARAMETERS - 1); j >= 0; j--) {
            unsigned int bit = (row >> j) & 1;
            currentArg.push_back(Brace((AMOUNT_OF_PARAMETERS - 1 - j), (bit == 0)));
        }

        // processing of currentArg
        for (long long combination = 0; combination < AMOUNT_OF_VARIANTS; combination++) {
            std::vector<unsigned int> arguments;
            bool xorZero = false;
            for (unsigned int b = 0; b < AMOUNT_OF_PARAMETERS; b++) {
                unsigned int currentBraceBit = (combination >> (AMOUNT_OF_PARAMETERS - 1 - b)) & 1;
                if (currentBraceBit == 0) {
                    arguments.push_back(currentArg[b].index());
                } else {
                    // currentBraceBit == 1 => take second place inside brace
                    if (currentArg[b].inverted()) { // +1
                        continue; // don't put this x, try next if any left
                    } else { // +0
                        xorZero = true;
                        break;
                    }
                }
            }
            if (xorZero) continue;
            long long currentIndex = 0;
            unsigned int size = arguments.size();
            for (unsigned int k = 0; k < size ; k++) {
                currentIndex += (1 << arguments[k]);
            }
            presenceTable[currentIndex] = !presenceTable[currentIndex];
        }
    }

    // analize presenceTable and show the result
    bool notFirst = false;
    for (int index = AMOUNT_OF_VARIANTS - 1; index >= 0; index--) {
        if (presenceTable[index]) {
            if (notFirst) {
                std::cout << " + ";
            } else {
                notFirst = true;
            }
            if (index == 0) {
                std::cout << "1";
            } else {
                unsigned int rightZeros = 0;
                for (unsigned int param = 0; param < AMOUNT_OF_PARAMETERS; param++) {
                    if (((index >> param) & 1) == 1) {
                        break;
                    }
                    rightZeros++;
                }
                std::cout << "x" << rightZeros;
            }
        }
    }
    std::cout << std::endl;
    std::cout << "--------------------------------------" << std::endl;
}


std::string Function::generateNonlinearFunc(unsigned int n) {
    const unsigned int LENGTH = 1 << n;
    std::string result;
    unsigned int amountOfOnes = 0;
    for (unsigned int i = 0; i < LENGTH; i++) {
        char newBit = '0' + (std::rand() % 2);
        result.append(1, newBit);
        if (newBit == '1') {
            amountOfOnes++;
        }
    }

    // Check whether generated function is balanced.
    // Need a correction if not balanced
    if (amountOfOnes != (LENGTH / 2)) {
        char charToCorrect;
        unsigned int bitsToCorrect;
        if (amountOfOnes < (LENGTH / 2)) {
            charToCorrect = '1';
            bitsToCorrect = LENGTH / 2 - amountOfOnes; 
        } else {
            charToCorrect = '0';
            bitsToCorrect = amountOfOnes - LENGTH / 2;
        }
        for (unsigned int j = 0; j < bitsToCorrect; ) {
            unsigned int pos = std::rand() % LENGTH;
            if (result.at(pos) != charToCorrect) {
                result.at(pos) = charToCorrect;
                j++;
            }
        }
    }

    return result;
}
