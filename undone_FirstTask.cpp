#include <iostream>
#include <vector>


static const unsigned int AMOUNT_OF_PARAMETERS = 2;
static const unsigned int AMOUNT_OF_VARIANTS = 1 << AMOUNT_OF_PARAMETERS;

struct Tuple {
// Examples of tuples: x0x3x4, x0x2, x1, ...
// Xs are zero-based
    private:
        // [i] == true => xi present
        bool xs[AMOUNT_OF_PARAMETERS];

    public:
        Tuple(long long number) {
            // TODO
            // Check all AMOUNT_OF_PARAMETERS
        }

        bool& operator[](unsigned int i) {
            return xs[i];
        }

        const bool& operator[](unsigned int i) const {
            return xs[i];
        }
};


struct Argument {
// Examples of Arguments: (x0)(!x1)(!x2)(x3), (x0)(x1)(x2)(x3), ...
// Xs are zero-based
    private:
        // [i] == true => xi not inverted
        bool xs[AMOUNT_OF_PARAMETERS];

    public:
        Argument(long long number) {
            // TODO
            // Check all AMOUNT_OF_PARAMETERS
        }

        bool& operator[](unsigned int i) {
            return xs[i];
        }

        const bool& operator[](unsigned int i) const {
            return xs[i];
        }
};


// start - zero-based
std::vector<long long> getPermutationsFrom(const Argument& argument, unsigned int start) {
    const bool x_inverted = !argument[start];
    const long long x_present     = 1 << start;
    const long long x_not_present = 0;
    //     inverted => (x ^ 1) => generates 2 tuples
    // not inverted => (x)     => generates 1 tuple

    if (start == AMOUNT_OF_PARAMETERS - 1) { // the last one
        if (x_inverted) {
            return {x_present, x_not_present};
        } else {
            return {x_present};
        }
    }

    std::vector<long long> tuples;
    const auto tail = getPermutationsFrom(argument, start + 1);

    // With xi present
    for (const long long& tuple : tail) {
        tuples.push_back(x_present | tuple);
    }
    // With xi not present (xi == 1)
    if (x_inverted) {
        for (const long long& tuple : tail) {
            tuples.push_back(x_not_present | tuple);
        }
    }

    return tuples;
}


// (!x) === (x ^ 1)
void generateTuples(std::vector<Tuple>& result, const Argument& argument) {
    const std::vector<long long> tuples = getPermutationsFrom(argument, 0);
    for (const long long& tuple : tuples) {
        result.emplace_back(tuple);
    }
}






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
