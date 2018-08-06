#include <iostream>
#include <cstring>
#include <vector>


static const unsigned int AMOUNT_OF_PARAMETERS = 3;
static const unsigned int AMOUNT_OF_VARIANTS = 1 << AMOUNT_OF_PARAMETERS; 


struct Function {
    private:
        // bool overflow;
        char elements[AMOUNT_OF_VARIANTS];

    public:
        Function() {
            memset(elements, 0, sizeof(char) * AMOUNT_OF_VARIANTS);
        }

        Function (unsigned int function) {
            for (int i = AMOUNT_OF_VARIANTS - 1; i >= 0; i--) {
                elements[i] = function % 10;
                function /= 10;
            }
        }

        char& operator[](unsigned int index) {
            return elements[index];
        }

        const char& operator[](unsigned int index) const {
            return elements[index];
        }
};


// Linear function format is x(n),..x(1),x(0),1
struct LinearFunction {
    private:
        std::vector<unsigned int> elements;

    public:
        LinearFunction() {
            for (unsigned int i = 0; i < AMOUNT_OF_PARAMETERS; i++) {
                elements.push_back(0);
            }
        }

        LinearFunction (long long number) {
            // Extra one loop is added to include "1" as element of linear equation
            for (unsigned int i = 0; i <= AMOUNT_OF_PARAMETERS; i++) {
                if ((number & 1) == 1) {
                    elements.push_back(AMOUNT_OF_PARAMETERS - i);
                }
                number >>= 1;
            }
        }

        unsigned int& operator[](unsigned int index) {
            return elements[index];
        }

        const unsigned int& operator[](unsigned int index) const {
            return elements[index];
        }
};


long long convertDecToBin (unsigned int number) {
    long long binNumber = 0;
    int weight = 1;

    while (number != 0) {
        unsigned int bit = number % 2;
        number /= 2;
        binNumber += bit * weight;
        weight *= 10;
    }

    return binNumber;
}


// Method for generation of the Truth Table
void generateTruthTable (unsigned int amountOfVariants, std::vector<long long>& truthTable) {
    for (unsigned int i = 0; i < amountOfVariants; i++) {

    }

}
// Method for generation of the array of linear functions
void getArrayOfLinearFunctions (std::vector<LinearFunction>& array) {
    for (unsigned int i = 0; i < (AMOUNT_OF_VARIANTS * 2); i++){
        array[i] = LinearFunction(i);
    }
}


unsigned int getNonlinearityOfFunction (Function function) {

}

int main() {

    return 0;
} 
