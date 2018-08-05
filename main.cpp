#include <iostream>
#include <cstring>


static const unsigned int AMOUNT_OF_PARAMETERS = 4;
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
            for (unsigned int i = AMOUNT_OF_VARIANTS - 1; i >= 0; i--) {
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
        char elements[AMOUNT_OF_VARIANTS];

    public:
        LinearFunction() {
            memset(elements, 0, sizeof(char) * AMOUNT_OF_VARIANTS);
        }

        LinearFunction (unsigned int function) {
            for (unsigned int i = AMOUNT_OF_VARIANTS - 1; i >= 0; i--) {
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


// generate linear functions
unsigned int getNonlinearityOfFunction (Function function) {

}

int main() {

    return 0;
} 
