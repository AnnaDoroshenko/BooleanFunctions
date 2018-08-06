#include <iostream>
#include <cstring>
#include <vector>


static const unsigned int AMOUNT_OF_PARAMETERS = 4;
static const unsigned int AMOUNT_OF_VARIANTS = 1 << AMOUNT_OF_PARAMETERS; 
std::vector<long long> arrayOfLinearFunctions;

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
            for (unsigned int i = 0; i < (AMOUNT_OF_PARAMETERS + 1); i++) {
                if ((number & 1) == 1) {
                    arrayOfLinearFunctions.push_back(AMOUNT_OF_PARAMETERS - 1);
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


// generate linear functions
unsigned int getNonlinearityOfFunction (Function function) {

}

int main() {

    return 0;
} 
