#include <iostream>
#include <math.h> // pow()

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

int main() {
    const unsigned int AMOUNT_OF_PARAMETERS = 2;
    const unsigned int AMOUNT_OF_VARIANTS = pow(2, AMOUNT_OF_PARAMETERS);
    // filling of the function table
    unsigned int table[AMOUNT_OF_VARIANTS] = {0, 1, 1, 0};

    // analysis of the function table
    for (unsigned int i = 0; i < AMOUNT_OF_VARIANTS; i++) {
        if (table[i] == 0) {
            continue;
        }
        for (unsigned int j = 0; j < AMOUNT_OF_PARAMETERS; j++) {
            long long currentRow = convertDecToBin(i);
            // analysis of the current row of function table
            if ((currentRow % 10) == 0) {
                continue;
            }
            /* str += "x" + std::to_string(j); */

        }
        if (table[i] == 1) {


        }
    }

    return 0;
} 
