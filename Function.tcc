#ifndef FUNCTION_TCC
#define FUNCTION_TCC

#include "Function.h"


template<std::size_t PARAMETERS>
Function<PARAMETERS>::Function(std::bitset<ROWS(PARAMETERS)>&& values) : values(values) {
    std::cout << "--------------------------------------" << std::endl;
    std::cout << "Input: " << " ... was here ..." << std::endl;
    std::cout << "--------------------------------------" << std::endl;
}

// Function::Function(std::string input) :
//     AMOUNT_OF_PARAMETERS(log2(input.size())),
//     AMOUNT_OF_ROWS(input.size()),
//     AMOUNT_OF_LAMBDA_FUNCTIONS(AMOUNT_OF_ROWS * 2 - 2) {
//         std::cout << "--------------------------------------" << std::endl;
//         std::cout << "Input: " << " ... was here ..." << std::endl;
//         std::cout << "--------------------------------------" << std::endl;
//
//         unsigned int s = input.size();
//         while ((s & 1) == 0) s >>= 1;
//         if ((s >> 1) != 0) {
//             throw std::invalid_argument("Invalid amount of function arguments passed. Terminating");
//         }
//
//         unsigned int countOnes = 0;
//         unsigned int countZeros = 0;
//         for (unsigned int bit = 0; bit < input.size(); bit++) {
//             if (input[bit] == '1') {
//                 countOnes++;
//             } else {
//                 countZeros++;
//             }
//         }
//         if (countOnes != countZeros) {
//             std::cout << "countOnes = " << countOnes << std::endl;
//             std::cout << "countZeros = " << countZeros << std::endl;
//             throw std::invalid_argument("Input function is not balanced. Terminating");
//         }
//
//         for (unsigned int i = 0; i < AMOUNT_OF_ROWS; i++) {
//             const unsigned int currentArgument = input.at(i) - '0';
//             values.push_back(currentArgument);
//         }
// }

template<std::size_t PARAMETERS>
bool Function<PARAMETERS>::operator[](unsigned int index) const {
    return values[index];
}


// Linear function format is x(0),x(1),..x(n) [1,x(0),x(1),..x(n)]
template<std::size_t PARAMETERS>
Function<PARAMETERS>::LinearFunction::LinearFunction() {};

template<std::size_t PARAMETERS>
Function<PARAMETERS>::LinearFunction::LinearFunction(long long number) {
    unsigned int index = 0;
    while (number != 0) {
        if ((number & 1) == 1) {
            indices.push_back(index);
        }
        number >>= 1;
        index++;
    }
};

template<std::size_t PARAMETERS>
unsigned int Function<PARAMETERS>::LinearFunction::size() const {
    return indices.size();
}

template<std::size_t PARAMETERS>
unsigned int& Function<PARAMETERS>::LinearFunction::operator[](unsigned int index) {
    return indices[index];
}

template<std::size_t PARAMETERS>
const unsigned int& Function<PARAMETERS>::LinearFunction::operator[](unsigned int index) const {
    return indices[index];
}


template<std::size_t PARAMETERS>
std::bitset<ROWS(PARAMETERS)> Function<PARAMETERS>::getLambdaFunction(unsigned int index) const {
    const auto linearFunction = LinearFunction(index);
    std::bitset<ROWS(PARAMETERS)> lambdaFunction;
    for (unsigned int i = 0; i < ROWS(PARAMETERS); i++) {
        unsigned int currentResult = 0;
        unsigned int j = 0;
        if (linearFunction[0] == 0) {
            currentResult = 1;
            j++;
        }
        for ( ; j < linearFunction.size(); j++) {
            currentResult ^= (i >> (linearFunction[j] - 1)) & 1;
        }
        lambdaFunction[i] = currentResult;
    }

    return lambdaFunction;
}


// Calculates Hamming distance between the input functions
// (and Truth Table representation of linear function can be printed)
// Be careful about input: [x(0), x(1), .. x(n), 1]
// For example, f=x3 => 0...00100, so function expects 4
template<std::size_t PARAMETERS>
unsigned int Function<PARAMETERS>::calculateH(std::vector<unsigned int>& indices) const {
    unsigned int decForm = 0;
    for (unsigned int index : indices) {
        decForm |= 1 << (PARAMETERS - index);
    }
    const auto lambdaFunction = getLambdaFunction(decForm);

    // For Truth Table representation uncomment next loop
    /* for (unsigned int i = 0; i < lambdaFunction.size(); i++) { */
    /*     std::cout << lambdaFunction[i]; */
    /* } */
    /* std::cout << std::endl; */

    unsigned int nonlinearity = 0;
    for (unsigned int j = 0; j < lambdaFunction.size(); j++) {
        if (values[j] != lambdaFunction[j]) {
            nonlinearity++;
        }
    }
    if (nonlinearity > (ROWS(PARAMETERS) >> 1)) {
        nonlinearity = ROWS(PARAMETERS) - nonlinearity;
    }
    /* std::cout << " H = " << nonlinearity << std::endl; */

    return nonlinearity;
}

template<std::size_t PARAMETERS>
unsigned int Function<PARAMETERS>::calculateMinH(
        const std::array<std::pair<unsigned int, double>, PARAMETERS>&& sortedStats) const {
    std::array<unsigned int, PARAMETERS> sorted;
    std::vector<unsigned int> distances;
    // sorted.reserve(AMOUNT_OF_PARAMETERS);
    distances.reserve(PARAMETERS);
    for (unsigned int i = 0; i < sortedStats.size(); i++) {
        sorted[i] = sortedStats[i].first;
        // sorted.push_back(stat.first);
    }
    std::cout << "sorted = [ ";
    for (unsigned int s : sorted) {
        std::cout << "x" << s << " ";
    }
    std::cout << "]" << std::endl;
    std::cout << "======================================" << std::endl;

    std::vector<std::vector<unsigned int>> selected;
    selected.push_back({sorted[0]});
        /* std::vector<long long> truthTable; */
        /* generateTruthTable(truthTable); */
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
    std::cout << "amount of args = " << PARAMETERS << std::endl;
    std::cout << "nonlinearity = " << minH << std::endl;
    /* std::cout << "passes = " << passes << std::endl; */
    unsigned int realPasses = PARAMETERS + passes;
    std::cout << "passes = " << realPasses << std::endl;

    return realPasses;
    /* return sorted; */
}


template<std::size_t PARAMETERS>
bool Function<PARAMETERS>::alreadyAdded(
        const std::vector<std::vector<unsigned int>>& selected, unsigned int test) const {
    for (auto& sel : selected) {
        if ((sel.size() == 1) && (sel[0] == test)) return true;
    }

    return false;
}


// Calculation of the Hamming distances between the input function
// and each of lambda functions. Then finding min distance.
// Also the closest linear functions can be shown
/* unsigned int Function::getMinDistance ( */
template<std::size_t PARAMETERS>
void Function<PARAMETERS>::calculateNonlinearity() const {
    unsigned int nonlinearity = std::numeric_limits<unsigned int>::max();
    std::vector<std::bitset<ROWS(PARAMETERS)>> arrayOfLambdaFunctions;
    for (unsigned int i = 2; i < LAMBDA_FUNCTIONS(PARAMETERS) + 2; i++) {
        const auto currentLambdaFunction = getLambdaFunction(i);
        const unsigned int currentNonlinearity = [&currentLambdaFunction, this](){
            unsigned int currentNonlinearity = 0;
            for (unsigned int j = 0; j < currentLambdaFunction.size(); j++) {
                if (values[j] != currentLambdaFunction[j]) {
                    currentNonlinearity++;
                }
            }
            return currentNonlinearity;
        }();

        if (currentNonlinearity < nonlinearity) {
            nonlinearity = currentNonlinearity;
            arrayOfLambdaFunctions.clear();
        }
        if (currentNonlinearity == nonlinearity) {
            arrayOfLambdaFunctions.push_back(std::move(currentLambdaFunction));
        }
    }

    std::cout << "Nonlinearity = " << nonlinearity << std::endl;
    std::cout << "--------------------------------------" << std::endl;

    for (const auto& minLambdaFunction : arrayOfLambdaFunctions) {
       for (unsigned int j = 0; j < minLambdaFunction.size(); j++) {
           std::cout << minLambdaFunction[j];
       }
       std::cout << std::endl;
    // TODO
       getLinearFunction(minLambdaFunction);
    }
}


// Format of a tuple is {number1, number2}
// numbers differ by one bit (for example, {100, 101})
template<std::size_t PARAMETERS>
Function<PARAMETERS>::Tuple::Tuple() : number1(0), number2(0) {}

template<std::size_t PARAMETERS>
Function<PARAMETERS>::Tuple::Tuple(long long number1, long long number2) : number1(number1), number2(number2) {}

template<std::size_t PARAMETERS>
long long& Function<PARAMETERS>::Tuple::operator[](unsigned int index) {
    assert(index <= 1);
    return (index == 0 ? number1 : number2);
}

template<std::size_t PARAMETERS>
const long long& Function<PARAMETERS>::Tuple::operator[](unsigned int index) const {
    assert(index <= 1);
    return (index == 0 ? number1 : number2);
}


// Base number is a number, from which we get inverted number by one exact x.
// Base number and inverted number form tuple.
// For example, 000 - base number, 001 - number inverted by x2
template<std::size_t PARAMETERS>
long long Function<PARAMETERS>::getBaseNumber(unsigned int parameter, unsigned int index) const {
    const long long chunkSize = 1 << parameter;
    const long long chunkIndex = index / chunkSize;
    const long long chunkShift = index % chunkSize;
    return chunkIndex * (chunkSize << 1) + chunkShift;
}


template<std::size_t PARAMETERS>
typename Function<PARAMETERS>::Tuple Function<PARAMETERS>::getTuple(long long baseNumber, unsigned int bitToInvert) const {
    return Tuple(baseNumber, baseNumber ^ (1 << bitToInvert));
}


/* std::vector<double> Function::getStatistics() { */
/*     std::vector<double> statisticArray; */
/*     statisticArray.reserve(AMOUNT_OF_PARAMETERS); */
/*     std::vector<std::vector<Tuple>> arrayOfTuples; */
/*     getArrayOfTuples(arrayOfTuples); */
/*     for (unsigned int i = 0; i < AMOUNT_OF_PARAMETERS; i++) { */
/*         unsigned int count = 0; */
/*         const std::vector<Tuple>& currentVectorOfArrayOfTuples = arrayOfTuples[i]; */
/*         for (unsigned int j = 0; j < (AMOUNT_OF_ROWS / 2); j++) { */
/*             const Tuple& currentTuple = currentVectorOfArrayOfTuples[j]; */
/*             if (values[currentTuple[0]] != values[currentTuple[1]]) { */
/*                 count++; */
/*             } */
/*             #<{(| std::cout << values[currentTuple[0]] << ";" << values[currentTuple[1]] <<std::endl; |)}># */
/*         } */
/*         // To see key values of calculation uncomment next three lines */
/*         #<{(| std::cout << "count = " << count << std::endl; |)}># */
/*         #<{(| std::cout << "AMOUNT_OF_ROWS = " << AMOUNT_OF_ROWS << std::endl; |)}># */
/*         #<{(| std::cout << std::endl; |)}># */
/*         statisticArray.push_back(static_cast<double>(count) / (AMOUNT_OF_ROWS / 2)); */
/*         std::cout << "x" << i << " => " << statisticArray[i] << std::endl; */
/*     } */
/*     std::cout << "--------------------------------------" << std::endl; */
/*  */
/*     return statisticArray; */
/* } */


template<std::size_t PARAMETERS>
std::array<std::pair<unsigned int, double>, PARAMETERS> Function<PARAMETERS>::getSortedStatistics() const {
    using Interest = std::pair<unsigned int, double>;
    std::array<Interest, PARAMETERS> statisticArray;
    for (unsigned int i = 0; i < PARAMETERS; i++) {
        unsigned int count = 0;
        for (unsigned int j = 0; j < (ROWS(PARAMETERS) >> 1); j++) {
            const auto currentTuple = getTuple(getBaseNumber(i, j), i);
            if (values[currentTuple[0]] != values[currentTuple[1]]) {
                count++;
            }
        }
        statisticArray[i] = Interest(i, static_cast<double>(count) / (ROWS(PARAMETERS) >> 1));
    }

    std::sort(statisticArray.begin(), statisticArray.end(),
        [](const Interest &left, const Interest &right) {return right.second < left.second;}
    );

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
template<std::size_t PARAMETERS>
Function<PARAMETERS>::Brace::Brace(unsigned int arg, bool isInverted) : elem(arg, isInverted) {}


template<std::size_t PARAMETERS>
Function<PARAMETERS>::Brace::Brace() : Brace(0, false) {}


template<std::size_t PARAMETERS>
unsigned int Function<PARAMETERS>::Brace::index() const {
    return elem.first;
}


template<std::size_t PARAMETERS>
bool Function<PARAMETERS>::Brace::inverted() const {
    return elem.second;
}


// TODO
/* void Function::getLinearFunction(const std::bitset<ROWS()>& currentLamdaFunc) { */
/*     using Argument = std::vector<Brace>; */
/*     bool presenceTable[AMOUNT_OF_ROWS]; */
/*     for (unsigned int i = 0; i < AMOUNT_OF_ROWS; i++) presenceTable[i] = false; */
/*  */
/*     for (unsigned int i = 0; i < AMOUNT_OF_ROWS; i++) { */
/*         if (currentLamdaFunc[i] == 0) continue; */
/*         #<{(| long long row = truthTable[i]; |)}># */
/*         long long row = i; */
/*         Argument currentArg; */
/*         currentArg.reserve(AMOUNT_OF_PARAMETERS); */
/*         for (int j = (AMOUNT_OF_PARAMETERS - 1); j >= 0; j--) { */
/*             unsigned int bit = (row >> j) & 1; */
/*             currentArg.push_back(Brace((AMOUNT_OF_PARAMETERS - 1 - j), (bit == 0))); */
/*         } */
/*  */
/*         // processing of currentArg */
/*         for (long long combination = 0; combination < AMOUNT_OF_ROWS; combination++) { */
/*             std::vector<unsigned int> arguments; */
/*             bool xorZero = false; */
/*             for (unsigned int b = 0; b < AMOUNT_OF_PARAMETERS; b++) { */
/*                 unsigned int currentBraceBit = (combination >> (AMOUNT_OF_PARAMETERS - 1 - b)) & 1; */
/*                 if (currentBraceBit == 0) { */
/*                     arguments.push_back(currentArg[b].index()); */
/*                 } else { */
/*                     // currentBraceBit == 1 => take second place inside brace */
/*                     if (currentArg[b].inverted()) { // +1 */
/*                         continue; // don't put this x, try next if any left */
/*                     } else { // +0 */
/*                         xorZero = true; */
/*                         break; */
/*                     } */
/*                 } */
/*             } */
/*             if (xorZero) continue; */
/*             long long currentIndex = 0; */
/*             unsigned int size = arguments.size(); */
/*             for (unsigned int k = 0; k < size ; k++) { */
/*                 currentIndex += (1 << arguments[k]); */
/*             } */
/*             presenceTable[currentIndex] = !presenceTable[currentIndex]; */
/*         } */
/*     } */
/*  */
/*     // analize presenceTable and show the result */
/*     bool notFirst = false; */
/*     for (int index = AMOUNT_OF_ROWS - 1; index >= 0; index--) { */
/*         if (presenceTable[index]) { */
/*             if (notFirst) { */
/*                 std::cout << " + "; */
/*             } else { */
/*                 notFirst = true; */
/*             } */
/*             if (index == 0) { */
/*                 std::cout << "1"; */
/*             } else { */
/*                 unsigned int rightZeros = 0; */
/*                 for (unsigned int param = 0; param < AMOUNT_OF_PARAMETERS; param++) { */
/*                     if (((index >> param) & 1) == 1) { */
/*                         break; */
/*                     } */
/*                     rightZeros++; */
/*                 } */
/*                 std::cout << "x" << rightZeros; */
/*             } */
/*         } */
/*     } */
/*     std::cout << std::endl; */
/*     std::cout << "--------------------------------------" << std::endl; */
/* } */


template<std::size_t PARAMETERS>
Function<PARAMETERS> Function<PARAMETERS>::generateNonlinearFunc() {
    std::bitset<ROWS(PARAMETERS)> result;
    unsigned int amountOfOnes = 0;
    for (unsigned int i = 0; i < ROWS(PARAMETERS); i++) {
        const bool newBit = std::rand() % 2 == 0;
        result[i] = newBit;
        if (newBit) amountOfOnes++;
    }

    // Check whether generated function is balanced.
    // Need a correction if not balanced
    const bool newBit = amountOfOnes < (ROWS(PARAMETERS) >> 1);
    while (amountOfOnes != (ROWS(PARAMETERS) >> 1)) {
        unsigned int pos;
        do { // Keep looking until have found a wrong bit
            pos = std::rand() % ROWS(PARAMETERS);
        } while (result[pos] == newBit);
        result.flip(pos);
        amountOfOnes += newBit ? 1 : -1;
    }

    return Function<PARAMETERS>(std::move(result));
}

#endif
