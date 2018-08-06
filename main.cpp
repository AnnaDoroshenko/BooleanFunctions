#include <iostream>
#include <cstring>
#include <vector>


static const unsigned int AMOUNT_OF_PARAMETERS = 3;
static const unsigned int AMOUNT_OF_VARIANTS = 1 << AMOUNT_OF_PARAMETERS; 


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

        Function (long long function) {
            for (unsigned int i = 0; i < AMOUNT_OF_VARIANTS; i++) {
                elements.push_back(function % 10);
                function /= 10;
            }
        }

        unsigned int& operator[](unsigned int index) {
            return elements[index];
        }

        unsigned int& operator[](unsigned int index) const {
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

        unsigned int getSize() {
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
void generateTruthTable (const unsigned int amountOfVariants, std::vector<long long>& truthTable) {
    for (unsigned int i = 0; i < amountOfVariants; i++) {
        truthTable[i] = i;
    }
}


// Method for generation of the array of linear functions
void getArrayOfLinearFunctions (std::vector<LinearFunction>& array) {
    for (unsigned int i = 0; i < (AMOUNT_OF_VARIANTS * 2); i++){
        array[i] = LinearFunction(i);
    }
}


void getLambdaFunction (const std::vector<long long>& truthTable, LinearFunction linearFunction, std::vector<unsigned int>& lambdaFunction) {
    for (unsigned int i = 0; i < AMOUNT_OF_VARIANTS; i++) {
        unsigned int currentResult = 0;
        unsigned int j = 0;
        if (linearFunction[0] == AMOUNT_OF_PARAMETERS) {
            currentResult = 1;
            j++;
        }
        for ( ; j < linearFunction.getSize(); j++) {
            if ((truthTable[i] >> (AMOUNT_OF_PARAMETERS - 1 - linearFunction[j])) & 1) {
                currentResult ^= currentResult;
            }
        }
        lambdaFunction[i] = currentResult;
    }
}
unsigned int getNonlinearityOfFunction (Function function) {

}

int main() {

    std::vector<LinearFunction> array;
    getArrayOfLinearFunctions(array);
    long long number = 0;
    for(unsigned int i = 0; i < array.size(); i++) {
        LinearFunction linFunc(i);
        for (unsigned int j = 0; j < linFunc.getSize(); j++) {
            std::cout << linFunc[j]; 
        }
        std::cout << std::endl;
        number++;
    }

    return 0;
} 
