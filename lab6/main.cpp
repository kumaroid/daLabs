#include "big_int.h"

const char OP_PLUS = '+';
const char OP_MINUS = '-';
const char OP_MULT = '*';
const char OP_DIV = '/';
const char OP_POW = '^';
const char OP_GREAT = '>';
const char OP_LESS = '<';
const char OP_EQUAL = '=';

int main() {
    std::string s1, s2;
    char operand;

    while (std::cin >> s1 >> s2 >> operand) {
        TBigInt n1(s1), n2(s2);
        TBigInt res;
        switch (operand) {
        case OP_PLUS:
            res = n1 + n2;
            res.Print();
            break;
        case OP_MINUS:
            if (n1 < n2) {
                std::cout << "Error" << '\n';
            } else {
                res = n1 - n2;
                res.Print();
            }
            break;
        case OP_MULT:
            n1 = n1.KaratsubaMultiply(n2);
            n1.Print();
            break;
        case OP_DIV:
            if (n2 == TBigInt(0)) {
                 std::cout << "Error" << '\n';
            } else {
                res = n1 / n2;
                res.Print();
            }
            break;
        case OP_POW:
            if (n1 == TBigInt(0)) {
                if (n2 == TBigInt(0)) {
                    std::cout << "Error" << '\n';
                } else {
                    std::cout << "0" << '\n';
                }
            } else if (n1 == TBigInt(1)) {
                std::cout << "1" << '\n';
            } else {
                res = n1.Pow(n2);
                res.Print();
            }
            break;
        case OP_GREAT:
            if (n1 > n2) {
                std::cout << "true" << '\n';
            } else {
                std::cout << "false" << '\n';
            }
            break;
        case OP_LESS:
            if (n1 < n2) {
                std::cout << "true" << '\n';
            } else {
                std::cout << "false" << '\n';
            }
            break;
        case OP_EQUAL:
            if (n1 == n2) {
                std::cout << "true" << '\n';
            } else {
                std::cout << "false" << '\n';
            }
            break;
        }
    }

    return 0;
}