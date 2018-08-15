#include <iostream>
#include <exception>
#include "Function.h"
#include <vector>
#include <bitset>


static const unsigned int AMOUNT_OF_ARGUMENTS = 5;
static const unsigned int AMOUNT_OF_VARIANTS = 1 << AMOUNT_OF_ARGUMENTS;

// Format of a tuple is {number1, number2}
// numbers differ by one bit (for example, {100, 101})
struct Tuple {
    private:
        std::vector<long long> elements;

    public:
        Tuple() {
            elements = {0, 0};
        }

        Tuple(long long number1, long long number2) {
            elements.push_back(number1);
            elements.push_back(number2);
        }

        long long& operator[](unsigned int index) {
            return elements[index];
        }

        const long long& operator[](unsigned int index) const {
            return elements[index];
        }
};


// Base number is a number, from which we get inverted number by one exact x.
// Base number and inverted number form tuple.
// For example, 000 - base number, 001 - number inverted by x2
std::vector<std::vector<long long>> getBaseNumbers() {
    std::vector<std::vector<long long>> vectorOfBaseNumbers;
    vectorOfBaseNumbers.reserve(AMOUNT_OF_ARGUMENTS);
    for (unsigned int i = 0; i < AMOUNT_OF_ARGUMENTS; i++) {
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


std::vector<Tuple> getTuples (
        const unsigned int& index, 
        const std::vector<long long>& baseNumbers) {
    std::vector<Tuple> tuples;
    tuples.reserve(AMOUNT_OF_VARIANTS / 2);
    const long long DIFFERENCE = 1 << index;
    for (unsigned int i = 0; i < (AMOUNT_OF_VARIANTS / 2); i++) {
        const long long number = baseNumbers[i];
        Tuple currentTuple(number, (number ^ DIFFERENCE));
        tuples.push_back(currentTuple);
        /* std::cout << "{" << std::bitset<AMOUNT_OF_ARGUMENTS>(currentTuple[0]) << ", " << std::bitset<AMOUNT_OF_ARGUMENTS>(currentTuple[1]) << "}" << std::endl; */
    }

    return tuples;
}


std::vector<std::vector<Tuple>> getArrayOfTuples () {
    std::vector<std::vector<long long>> vectorOfBaseNumbers = getBaseNumbers();
    std::vector<std::vector<Tuple>> result;
    result.reserve(AMOUNT_OF_ARGUMENTS);
        for (unsigned int i = 0; i < AMOUNT_OF_ARGUMENTS; i++) {
        result[i] = getTuples((AMOUNT_OF_ARGUMENTS - 1 - i), vectorOfBaseNumbers[i]);
    }

    return result;
}


int main() {
    // try {
    //     Function testFunction("11001001");
    //     std::cout << testFunction.calculateNonlinearity() << std::endl;
    // } catch(const std::invalid_argument& e) {
    //     std::cerr << e.what() << std::endl;
    // }

    getArrayOfTuples();

    return 0;
} 
