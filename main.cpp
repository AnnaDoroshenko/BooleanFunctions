#include <iostream>
#include <cstring>

// '0'..'9' -> 0..9
// otherwise -> -1
int toInt(char c) {
    if ('0' <= c && c <= '9') {
        return c - '0';
    }

    return -1;
}


static const unsigned int MAX_DIGITS = 9;

struct Number {
    private:
        bool overflow;
        char digits[MAX_DIGITS];

    public:
        Number() : overflow(false) {
            memset(digits, 0, sizeof(char) * MAX_DIGITS);
        }

        Number (unsigned int number) : overflow(false) {
            for (unsigned int i = MAX_DIGITS; i > 0; i--) {
                digits[i - 1] = number % 10;
                number /= 10;
            }
        }

        Number (const std::string& number) : overflow(false) {
            // TODO: init from "number"
            // Note: exit(-1) with a message if number.size() > MAX_DIGITS;
        }

        bool isOverflow() const {
            return overflow;
        }

        // TODO: learn about operator overloading in C++.
        // Note: the following two almost always come in pairs.
        char& operator[](unsigned int index) {
            return digits[index];
        }

        const char& operator[](unsigned int index) const {
            return digits[index];
        }

        friend std::ostream& operator<<(std::ostream& stream, const Number& number);
};


std::ostream& operator<<(std::ostream& stream, const Number& number) {
    unsigned int index = 0;
    while ((number[index] == 0) && (index <  MAX_DIGITS - 1)) {
        index++;
    }
    for (unsigned int i = index; i < MAX_DIGITS; i++) {
        stream << static_cast<unsigned int> (number[i]);
    }

    return stream;
}


// Don't forget about the overflow flag
Number operator+(const Number& number1, const Number& number2) {
    // TODO
}



int main() {

    Number number1( 0 );
    std::cout << number1 << std::endl;
    /* Number number2("999999999"); */
    /* std::cout << number1 << " + " << number2 << " = " << (number1 + number2) << std::endl; */

    return 0;
} 
